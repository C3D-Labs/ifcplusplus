#include <unordered_set>
#include <locale>
#include <codecvt>
#include <thread>
#include <chrono>

#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcRepresentation.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/model/BuildingGuid.h>
#include <ifcpp/reader/ReaderSTEP.h>

#include <ifcpp/geometry/c3d/GeometryConverter.h>
#include <ifcpp/geometry/c3d/PlacementConverter.h>

#include <c3dservice-api/ApiConfiguration.h>
#include <c3dservice-api/ApiClient.h>
#include <c3dservice-api/api/MonitorApi.h>
#include <c3dservice-api/api/CacheApi.h>

#include "conv_exchange_settings.h"
#include "conv_model_exchange.h"
#include "tool_enabler.h"
#include "attr_common_attribute.h"

#include "base64.h"
#include "boost/program_options.hpp"


using namespace org::openapitools::client;
using namespace std::chrono_literals;
namespace po = boost::program_options;


struct NodeTreeItem
{
    std::string item_uuid;
    std::string rep_uuid;
    std::shared_ptr<api::ComposeModelNode> web_item;
    SPtr<MbItem>                           math_item;
};


struct GeometryMap
{
public:
    std::shared_ptr<GeometryConverter> m_converter;
public:
    std::shared_ptr<ProductShapeData> operator() (const std::wstring& global_id) const
    {
        return m_converter ? m_converter->GetProductShape(global_id) : std::shared_ptr<ProductShapeData>{};
    }
};


NodeTreeItem resolveTreeItems(shared_ptr<BuildingObject> obj, std::unordered_set<int>& set_visited, const GeometryMap& geoms)
{
    NodeTreeItem item;
    
    shared_ptr<IfcObjectDefinition> obj_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
    if (obj_def)
    {
        std::map<utility::string_t, utility::string_t> attrs;
        std::vector<std::shared_ptr<model::ComposeModelNode>> web_children;
        std::vector<SPtr<MbItem>> math_children;
        MbPlacement3D placement;

        if (set_visited.find(obj_def->m_entity_id) != set_visited.end())
        {
            return item;
        }
        set_visited.insert(obj_def->m_entity_id);

        item.web_item = std::shared_ptr<api::ComposeModelNode>(new api::ComposeModelNode);
        
        //item->m_ifc_class_name = obj_def->className();

        // access some attributes of IfcObjectDefinition
        if (obj_def->m_GlobalId)
        {
            //item->m_entity_guid = obj_def->m_GlobalId->m_value;
        }

        static const std::string attr_name = "name";
        static const std::string attr_id   = "id";

        if (obj_def->m_Name)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
            attrs[attr_name] = convert.to_bytes(obj_def->m_Name->m_value);
        }

        if (obj_def->m_Description)
        {
            //std::wcout << obj_def->m_Description->m_value << std::endl;
            //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
            //item->getAttrs()[attr_id] = convert.to_bytes(obj_def->m_Description->m_value);
        }
        
        // check if there are child elements of current IfcObjectDefinition
        // child elements can be related to current IfcObjectDefinition either by IfcRelAggregates, or IfcRelContainedInSpatialStructure, see IFC doc
        if (obj_def->m_IsDecomposedBy_inverse.size() > 0)
        {
            // use inverse attribute to navigate to child elements:
            std::vector<weak_ptr<IfcRelAggregates> >& vec_IsDecomposedBy = obj_def->m_IsDecomposedBy_inverse;
            for ( auto it = vec_IsDecomposedBy.begin(); it != vec_IsDecomposedBy.end(); ++it)
            {
                shared_ptr<IfcRelAggregates> rel_agg(*it);
                std::vector<shared_ptr<IfcObjectDefinition> >& vec_related_objects = rel_agg->m_RelatedObjects;
                for (shared_ptr<IfcObjectDefinition> child_obj_def : vec_related_objects)
                {
                    auto child_item = resolveTreeItems(child_obj_def, set_visited, geoms);
                    if (child_item.web_item)
                    {
                        web_children.push_back(child_item.web_item);
                    }
                    if(child_item.math_item)
                    {
                        math_children.push_back(child_item.math_item);
                    }
                }
            }
        }
        
        shared_ptr<IfcSpatialStructureElement> spatial_ele = dynamic_pointer_cast<IfcSpatialStructureElement>(obj_def);
        if (spatial_ele)
        {
            // use inverse attribute to navigate to child elements:
            std::vector<weak_ptr<IfcRelContainedInSpatialStructure> >& vec_contained = spatial_ele->m_ContainsElements_inverse;
            if (vec_contained.size() > 0)
            {
                for (auto it_rel_contained = vec_contained.begin(); it_rel_contained != vec_contained.end(); ++it_rel_contained)
                {
                    shared_ptr<IfcRelContainedInSpatialStructure> rel_contained(*it_rel_contained);
                    std::vector<shared_ptr<IfcProduct> >& vec_related_elements = rel_contained->m_RelatedElements;

                    for (shared_ptr<IfcProduct> related_product : vec_related_elements)
                    {
                        auto child_item = resolveTreeItems(related_product, set_visited, geoms);
                        if (child_item.web_item)
                        {
                            web_children.push_back(child_item.web_item);
                        }

                        if(child_item.math_item)
                        {
                            math_children.push_back(child_item.math_item);
                        }
                    }
                }
            }
        }

        std::shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(obj_def);
        if(ifc_product)
        {
            if( auto&& shape = geoms(ifc_product->m_GlobalId->toString()) )
            {

                if(shape->m_pMathItem){
                    
                    auto pMathItem = shape->m_pMathItem;
                    // assign the 3d geometry to the node
                    item.rep_uuid = shape->m_math_uuid;
                    // generate UUID for the node, to using the item in Web visualization
                    item.item_uuid = createGUID32();

                    placement.Init(shape->getTransform());

                    if(placement.IsTranslation()||placement.IsRotation()){
                        pMathItem = SPtr<MbInstance>(new MbInstance(*shape->m_pMathItem, placement));
                        math_children.push_back(pMathItem);
                    } else {
                        math_children.push_back(shape->m_pMathItem);
                    }

                    C3DCharEncodingTransformerLocale conv;
                    c3d::string_t value;
                    conv.StdToC3D(item.item_uuid, value);
                    SPtr<MbStringAttribute> ptr(new MbStringAttribute(L"product_uuid",false, value));
                    pMathItem->AddAttribute(*ptr);
                }
            }

            //GeomUtils::GetC3dPlacement3D(dynamic_pointer_cast<IfcLocalPlacement>(ifc_product->m_ObjectPlacement), placement);
        }

        if( !math_children.empty() )
        {
            if(math_children.size() == 1)
            {
                item.math_item = math_children[0];
            } else {
                SPtr<MbAssembly> pAssm(new MbAssembly);

                for( auto&& pItem : math_children )
                    pAssm->AddItem(*pItem);

                item.math_item = SPtr<MbInstance>(new MbInstance(*pAssm, {}));
            }
        }

        item.web_item->setChildren(web_children);
        item.web_item->setAttrs(attrs);
        item.web_item->setUuid(item.item_uuid);
        item.web_item->setRepresentation(item.rep_uuid);

        if(placement.IsTranslation() || placement.IsRotation()){
            MbMatrix3D from = placement.GetMatrixFrom();
            item.web_item->setMatrix({
                static_cast<float>(from.GetEl()[0]),
                static_cast<float>(from.GetEl()[1]),
                static_cast<float>(from.GetEl()[2]),
                static_cast<float>(from.GetEl()[3]),
                static_cast<float>(from.GetEl()[4]),
                static_cast<float>(from.GetEl()[5]),
                static_cast<float>(from.GetEl()[6]),
                static_cast<float>(from.GetEl()[7]),
                static_cast<float>(from.GetEl()[8]),
                static_cast<float>(from.GetEl()[9]),
                static_cast<float>(from.GetEl()[10]),
                static_cast<float>(from.GetEl()[11]),
                static_cast<float>(from.GetEl()[12]),
                static_cast<float>(from.GetEl()[13]),
                static_cast<float>(from.GetEl()[14]),
                static_cast<float>(from.GetEl()[15]),
            });
        }
    }

    return item;
}

template<typename Func>
pplx::task<void> doWhile(Func func)
{
  //static_assert(
  //  std::is_same_v<decltype(func()), bool> ||
  //  std::is_same_v<decltype(func()), pplx::task<bool>>);

  return pplx::create_task(func)

    .then([func](bool needToContinue)
    {
      if (needToContinue)
        return doWhile(func);

      return pplx::task_from_result();
    });
}

int main(int ac, char *av[])
{
    // Declare the supported options.
    po::options_description desc("Allowed options");

    desc.add_options()
        ("help", "produce help message")
        ("input-file,f", po::value<std::string>(), "input file")
        ("license-sign", po::value<std::string>(), "license signature")
        ("license-key", po::value<std::string>(), "license key")
        ("service-url,s", po::value<std::string>(), "service-api url addres")
    ;
    po::positional_options_description p;
    p.add("input-file", -1);
    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
            options(desc).positional(p).run(), vm);

    std::wstring input_file = L"example.ifc";
    std::string service_addr = "http://127.0.0.1:12344/v1";
    std::string strKey = "";
    std::string strSignature = "";


    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("license-key")) {
        strKey = vm["license-key"].as<std::string>();;
    }

    if (vm.count("license-sign")) {
        strSignature = vm["license-sign"].as<std::string>();;
    }

    if (vm.count("input-file")){
        using convert_t = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_t, wchar_t> strconverter;
        input_file = strconverter.from_bytes(vm["input-file"].as<std::string>());
    }
        
    if (vm.count("service-url"))
        service_addr = vm["service-url"].as<std::string>();

    ::EnableMathModules(strKey.c_str(), static_cast<int>(strKey.length()), strSignature.c_str(), static_cast<int>(strSignature.length()));
    std::cout << "IsMathModelerEnable: " << (IsMathModelerEnable()?"True":"False") << std::endl;

    std::shared_ptr<api::ApiClient> apiClient(new api::ApiClient);
    std::shared_ptr<api::ApiConfiguration> apiConfig(new api::ApiConfiguration);
    apiConfig->setBaseUrl(service_addr);
    apiClient->setConfiguration(apiConfig);

    std::shared_ptr<api::MonitorApi>  monitor_api(new api::MonitorApi(apiClient));
    std::shared_ptr<api::CacheApi>    cache_api(new api::CacheApi(apiClient));

    shared_ptr<BuildingModel>           ifc_model;
    shared_ptr<ReaderSTEP>              step_reader;
    std::shared_ptr<GeometryConverter>  ifc_geom_converter;


    std::string  model_uuid;
    NodeTreeItem root_item;

    auto task_  = //monitor_api->getStatus()
    pplx::create_task([]{})
    // check service
    .then([&]{
        std::cout << "Servce is active" << std::endl;
    })
    // load model
    .then([&]{
        std::wcout << L"Loading file " << input_file << std::endl;
        ifc_model.reset(new BuildingModel());
        step_reader.reset(new ReaderSTEP());
        step_reader->loadModelFromFile( input_file, ifc_model);
    })
    // build geometry
    .then([&]{
        std::cout << "Building geometry" << std::endl;
        ifc_geom_converter.reset(new GeometryConverter(ifc_model));
        ifc_geom_converter->convertGeometry();
    })
    // sending geometry
    .then([&]{
        std::cout << "Sending geometry to service" << std::endl;
        std::shared_ptr<model::CacheGeometry_request> request(new model::CacheGeometry_request());

        const bool sendToService = false;
        const bool saveToFile = false;
        for(auto&& shapeData :ifc_geom_converter->getShapeInputData())
        {
            auto pMathItem = shapeData.second->toMathItem();
            if(pMathItem)
            {
                // save MbItem to c3d buffer
                c3d::C3DExchangeBuffer buffer;
                MbeConvResType res = c3d::ExportIntoBuffer(*pMathItem, mxf_C3D, buffer);
                std::string geom_uuid = createGUID32();

                if(res != MbeConvResType::cnv_Success){
                    std::cerr << "Error save to buffer " << res << std::endl;
                    return;
                }

                if(sendToService) //send the c3d buffer like base64 to serivce
                {
                    std::string base64 = base64_encode(reinterpret_cast<const unsigned char*>(buffer.Data()), buffer.Count(), false );
                    request->setFileContent(base64);
                    request->setBuilder("c3d");
                    request->setUuid(geom_uuid);

                    cache_api->cacheGeometry(request)
                    .then([&](std::string uuidGeom){
                        geom_uuid = uuidGeom;
                        return doWhile([=]{
                            std::this_thread::sleep_for(20ms);
                            double progress = 0.0;
                            try{
                                auto status = cache_api->getGeometry(geom_uuid).get();
                                progress = status->getProgress();
                            }
                            catch(...)
                            {
                                throw;
                            }

                            return progress < 1.0;
                        });
                    }).get();
                }
                if(saveToFile) //save c3d buffer to file
                {
                    if(geom_uuid.empty()){
                        geom_uuid = createGUID32();
                    }
                    
                    std::ofstream file("./c3d/" + geom_uuid + ".c3d", std::ios::binary);
                    file.write(buffer.Data(), buffer.Count());
                }

                shapeData.second->m_math_uuid = geom_uuid;
            }
        }
    })
    // build structure
    .then([&]{
        GeometryMap geometry_map{
            ifc_geom_converter
        };

        std::unordered_set<int> set_visited;
        root_item = resolveTreeItems(ifc_model->getIfcProject(), set_visited, geometry_map);

        std::cout << "Building structure" << std::endl;
    })
    // sending structure
    .then([&]{
        std::cout << "Sending structure to service" << std::endl;

        const bool sendToService = false;
        const bool saveToFile = true;

        if(root_item.web_item && sendToService)
        {
            model_uuid = createGUID32();
            auto data = std::make_shared<api::CacheComposeModel_request>();
            data->setRoot(root_item.web_item);
            data->setUuid(model_uuid);

            auto task = cache_api->cacheComposeModel(data);
            model_uuid = task.get();
            std::cout << "model UUID: " << model_uuid << std::endl;
        }

        // save to json file:
        if(root_item.web_item && saveToFile)
        {
            auto jsonModel = root_item.web_item->toJson();

            if(model_uuid.empty()){
                model_uuid = createGUID32();
            }

            std::ofstream jsonFile(model_uuid + ".json");
            jsonModel.serialize(jsonFile);

            if(root_item.math_item)
                c3d::ExportIntoFile(*root_item.math_item, model_uuid + ".c3d");
        }

        return model_uuid;
    });

    /* Run the converter */
    try
    {
        model_uuid = task_.get();
    }
    catch(const api::ApiException& api)
    {
        std::cout << "Error: " << api.error_code().value() <<  std::endl
                  << api.error_code().message() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
