#include <unordered_set>
#include <locale>
#include <codecvt>

#include <ifcpp/IFC4/include/IfcBuildingStorey.h>
#include <ifcpp/IFC4/include/IfcGloballyUniqueId.h>
#include <ifcpp/IFC4/include/IfcLabel.h>
#include <ifcpp/IFC4/include/IfcObjectDefinition.h>
#include <ifcpp/IFC4/include/IfcProject.h>
#include <ifcpp/IFC4/include/IfcRelAggregates.h>
#include <ifcpp/IFC4/include/IfcRelContainedInSpatialStructure.h>
#include <ifcpp/IFC4/include/IfcText.h>
#include <ifcpp/model/BuildingModel.h>
#include <ifcpp/reader/ReaderSTEP.h>

#include <ifcpp/geometry/c3d/GeometryConverter.h>
#include <ifcpp/geometry/c3d/PlacementConverter.h>

#include <c3dservice-Api/ApiConfiguration.h>
#include <c3dservice-Api/ApiClient.h>
#include <c3dservice-Api/api/MonitorApi.h>
#include <c3dservice-Api/api/CacheApi.h>



using namespace org::openapitools::client;


std::shared_ptr<api::ComposeModelNode> resolveTreeItems(shared_ptr<BuildingObject> obj, std::unordered_set<int>& set_visited)
{
    //shared_ptr<MyIfcTreeItem> item;
    std::shared_ptr<api::ComposeModelNode> item(new api::ComposeModelNode);
    
    shared_ptr<IfcObjectDefinition> obj_def = dynamic_pointer_cast<IfcObjectDefinition>(obj);
    if (obj_def)
    {
        std::map<utility::string_t, utility::string_t> attrs;
        std::vector<std::shared_ptr<model::ComposeModelNode>> children;

        if (set_visited.find(obj_def->m_entity_id) != set_visited.end())
        {
            return nullptr;
        }
        set_visited.insert(obj_def->m_entity_id);

        //item = std::shared_ptr<MyIfcTreeItem>(new MyIfcTreeItem());
        item = std::shared_ptr<api::ComposeModelNode>(new api::ComposeModelNode);
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
                    auto child_tree_item = resolveTreeItems(child_obj_def, set_visited);
                    if (child_tree_item)
                    {
                        children.push_back(child_tree_item);
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
                        auto child_tree_item = resolveTreeItems(related_product, set_visited);
                        if (child_tree_item)
                        {
                            children.push_back(child_tree_item);
                        }
                    }
                }
            }
        }

        std::shared_ptr<IfcProduct> ifc_product = dynamic_pointer_cast<IfcProduct>(obj_def);
        if(ifc_product)
        {
            if(ifc_product->m_Representation)
            {
                //std::wcout << ifc_product->m_ObjectPlacement->toString() << std::endl;
                //item->setMatrix()

                // convert matrix
                //PlacementConverter placementConverter({});
                //std::shared_ptr<TransformData> transformData;
                //placementConverter.convertIfcObjectPlacement(ifc_product->m_ObjectPlacement, transformData);

                shared_ptr<IfcLocalPlacement> local_placement = dynamic_pointer_cast<IfcLocalPlacement>( ifc_product->m_ObjectPlacement );
                if(local_placement)
                {
                    std::wcout << local_placement->m_RelativePlacement->toString() << std::endl;
                }
            }
        }

        item->setChildren(children);
        item->setAttrs(attrs);
        
    }

    return item;
}

int main()
{
    std::shared_ptr<api::ApiClient> apiClient(new api::ApiClient);
    std::shared_ptr<api::ApiConfiguration> apiConfig(new api::ApiConfiguration);
    apiConfig->setBaseUrl("http://127.0.0.1:12344/v1");
    apiClient->setConfiguration(apiConfig);

    // testing service
    std::shared_ptr<api::MonitorApi>  monitor(new api::MonitorApi(apiClient));

    /*try
    {
        auto task = monitor->getStatus()
        .then([=](){
            std::cout << "servce is active" << std::endl;
        })
        .wait();
    }
    catch(const std::exception& e)
    {
        std::cout << "servce is not active" << std::endl 
        << e.what() << std::endl;
        return 1;
    }*/

    // 1: create an IFC model and a reader for IFC files in STEP format:
    shared_ptr<BuildingModel> ifc_model(new BuildingModel());
    shared_ptr<ReaderSTEP> step_reader(new ReaderSTEP());

    // 2: load the model:
    step_reader->loadModelFromFile( L"example.ifc", ifc_model);

    std::shared_ptr<GeometryConverter> geomConverter(new GeometryConverter(ifc_model));
    geomConverter->convertGeometry();

    // 4: traverse tree structure of model, starting at root object (IfcProject)
    shared_ptr<IfcProject> ifc_project = ifc_model->getIfcProject();
    std::unordered_set<int> set_visited;
    auto root_item = resolveTreeItems(ifc_project, set_visited);

    if(!root_item)
        return 1;

    // init 
    std::shared_ptr<api::CacheApi> cache(new api::CacheApi(apiClient));

    /*try
    {
        auto task = cache->cacheComposeModel(root_item)
        .then([=](pplx::task<std::string> uuid){
            std::cout << "Model is chached on service"<< std::endl 
            << "UUID: " << uuid.get() << std::endl;
        })
        .wait();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }*/

    return 0;
}
