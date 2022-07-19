/**
 * OpenAPI C3D-Service
 * Open API спецификация к REST API микросервиса C3D Web Vision.
 *
 * The version of the OpenAPI document: 1.2.1
 * Contact: info@c3dlabs.com
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 6.0.0.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */



#include "c3dservice-api/model/ComposeModelNode.h"

namespace org {
namespace openapitools {
namespace client {
namespace model {



ComposeModelNode::ComposeModelNode()
{
    m_Uuid = utility::conversions::to_string_t("");
    m_UuidIsSet = false;
    m_AttrsIsSet = false;
    m_Visibility = false;
    m_VisibilityIsSet = false;
    m_Representation = utility::conversions::to_string_t("");
    m_RepresentationIsSet = false;
    m_ChildrenIsSet = false;
    m_LocationIsSet = false;
    m_MatrixIsSet = false;
}

ComposeModelNode::~ComposeModelNode()
{
}

void ComposeModelNode::validate()
{
    // TODO: implement validation
}

web::json::value ComposeModelNode::toJson() const
{

    web::json::value val = web::json::value::object();
    
    if(m_UuidIsSet)
    {
        val[utility::conversions::to_string_t(U("uuid"))] = ModelBase::toJson(m_Uuid);
    }
    if(m_AttrsIsSet)
    {
        val[utility::conversions::to_string_t(U("attrs"))] = ModelBase::toJson(m_Attrs);
    }
    if(m_VisibilityIsSet)
    {
        val[utility::conversions::to_string_t(U("visibility"))] = ModelBase::toJson(m_Visibility);
    }
    if(m_RepresentationIsSet)
    {
        val[utility::conversions::to_string_t(U("representation"))] = ModelBase::toJson(m_Representation);
    }
    if(m_ChildrenIsSet)
    {
        val[utility::conversions::to_string_t(U("children"))] = ModelBase::toJson(m_Children);
    }
    if(m_LocationIsSet)
    {
        val[utility::conversions::to_string_t(U("location"))] = ModelBase::toJson(m_Location);
    }
    if(m_MatrixIsSet)
    {
        val[utility::conversions::to_string_t(U("matrix"))] = ModelBase::toJson(m_Matrix);
    }

    return val;
}

bool ComposeModelNode::fromJson(const web::json::value& val)
{
    bool ok = true;
    
    if(val.has_field(utility::conversions::to_string_t(U("uuid"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("uuid")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setUuid;
            ok &= ModelBase::fromJson(fieldValue, refVal_setUuid);
            setUuid(refVal_setUuid);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("attrs"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("attrs")));
        if(!fieldValue.is_null())
        {
            std::map<utility::string_t, utility::string_t> refVal_setAttrs;
            ok &= ModelBase::fromJson(fieldValue, refVal_setAttrs);
            setAttrs(refVal_setAttrs);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("visibility"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("visibility")));
        if(!fieldValue.is_null())
        {
            bool refVal_setVisibility;
            ok &= ModelBase::fromJson(fieldValue, refVal_setVisibility);
            setVisibility(refVal_setVisibility);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("representation"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("representation")));
        if(!fieldValue.is_null())
        {
            utility::string_t refVal_setRepresentation;
            ok &= ModelBase::fromJson(fieldValue, refVal_setRepresentation);
            setRepresentation(refVal_setRepresentation);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("children"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("children")));
        if(!fieldValue.is_null())
        {
            std::vector<std::shared_ptr<ComposeModelNode>> refVal_setChildren;
            ok &= ModelBase::fromJson(fieldValue, refVal_setChildren);
            setChildren(refVal_setChildren);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("location"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("location")));
        if(!fieldValue.is_null())
        {
            std::shared_ptr<GeometryLocation> refVal_setLocation;
            ok &= ModelBase::fromJson(fieldValue, refVal_setLocation);
            setLocation(refVal_setLocation);
        }
    }
    if(val.has_field(utility::conversions::to_string_t(U("matrix"))))
    {
        const web::json::value& fieldValue = val.at(utility::conversions::to_string_t(U("matrix")));
        if(!fieldValue.is_null())
        {
            std::vector<float> refVal_setMatrix;
            ok &= ModelBase::fromJson(fieldValue, refVal_setMatrix);
            setMatrix(refVal_setMatrix);
        }
    }
    return ok;
}

void ComposeModelNode::toMultipart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix) const
{
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }
    if(m_UuidIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("uuid")), m_Uuid));
    }
    if(m_AttrsIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("attrs")), m_Attrs));
    }
    if(m_VisibilityIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("visibility")), m_Visibility));
    }
    if(m_RepresentationIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("representation")), m_Representation));
    }
    if(m_ChildrenIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("children")), m_Children));
    }
    if(m_LocationIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("location")), m_Location));
    }
    if(m_MatrixIsSet)
    {
        multipart->add(ModelBase::toHttpContent(namePrefix + utility::conversions::to_string_t(U("matrix")), m_Matrix));
    }
}

bool ComposeModelNode::fromMultiPart(std::shared_ptr<MultipartFormData> multipart, const utility::string_t& prefix)
{
    bool ok = true;
    utility::string_t namePrefix = prefix;
    if(namePrefix.size() > 0 && namePrefix.substr(namePrefix.size() - 1) != utility::conversions::to_string_t(U(".")))
    {
        namePrefix += utility::conversions::to_string_t(U("."));
    }

    if(multipart->hasContent(utility::conversions::to_string_t(U("uuid"))))
    {
        utility::string_t refVal_setUuid;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("uuid"))), refVal_setUuid );
        setUuid(refVal_setUuid);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("attrs"))))
    {
        std::map<utility::string_t, utility::string_t> refVal_setAttrs;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("attrs"))), refVal_setAttrs );
        setAttrs(refVal_setAttrs);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("visibility"))))
    {
        bool refVal_setVisibility;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("visibility"))), refVal_setVisibility );
        setVisibility(refVal_setVisibility);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("representation"))))
    {
        utility::string_t refVal_setRepresentation;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("representation"))), refVal_setRepresentation );
        setRepresentation(refVal_setRepresentation);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("children"))))
    {
        std::vector<std::shared_ptr<ComposeModelNode>> refVal_setChildren;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("children"))), refVal_setChildren );
        setChildren(refVal_setChildren);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("location"))))
    {
        std::shared_ptr<GeometryLocation> refVal_setLocation;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("location"))), refVal_setLocation );
        setLocation(refVal_setLocation);
    }
    if(multipart->hasContent(utility::conversions::to_string_t(U("matrix"))))
    {
        std::vector<float> refVal_setMatrix;
        ok &= ModelBase::fromHttpContent(multipart->getContent(utility::conversions::to_string_t(U("matrix"))), refVal_setMatrix );
        setMatrix(refVal_setMatrix);
    }
    return ok;
}

utility::string_t ComposeModelNode::getUuid() const
{
    return m_Uuid;
}

void ComposeModelNode::setUuid(const utility::string_t& value)
{
    m_Uuid = value;
    m_UuidIsSet = true;
}

bool ComposeModelNode::uuidIsSet() const
{
    return m_UuidIsSet;
}

void ComposeModelNode::unsetUuid()
{
    m_UuidIsSet = false;
}
std::map<utility::string_t, utility::string_t>& ComposeModelNode::getAttrs()
{
    return m_Attrs;
}

void ComposeModelNode::setAttrs(const std::map<utility::string_t, utility::string_t>& value)
{
    m_Attrs = value;
    m_AttrsIsSet = true;
}

bool ComposeModelNode::attrsIsSet() const
{
    return m_AttrsIsSet;
}

void ComposeModelNode::unsetAttrs()
{
    m_AttrsIsSet = false;
}
bool ComposeModelNode::isVisibility() const
{
    return m_Visibility;
}

void ComposeModelNode::setVisibility(bool value)
{
    m_Visibility = value;
    m_VisibilityIsSet = true;
}

bool ComposeModelNode::visibilityIsSet() const
{
    return m_VisibilityIsSet;
}

void ComposeModelNode::unsetVisibility()
{
    m_VisibilityIsSet = false;
}
utility::string_t ComposeModelNode::getRepresentation() const
{
    return m_Representation;
}

void ComposeModelNode::setRepresentation(const utility::string_t& value)
{
    m_Representation = value;
    m_RepresentationIsSet = true;
}

bool ComposeModelNode::representationIsSet() const
{
    return m_RepresentationIsSet;
}

void ComposeModelNode::unsetRepresentation()
{
    m_RepresentationIsSet = false;
}
std::vector<std::shared_ptr<ComposeModelNode>>& ComposeModelNode::getChildren()
{
    return m_Children;
}

void ComposeModelNode::setChildren(const std::vector<std::shared_ptr<ComposeModelNode>>& value)
{
    m_Children = value;
    m_ChildrenIsSet = true;
}

bool ComposeModelNode::childrenIsSet() const
{
    return m_ChildrenIsSet;
}

void ComposeModelNode::unsetChildren()
{
    m_ChildrenIsSet = false;
}
std::shared_ptr<GeometryLocation> ComposeModelNode::getLocation() const
{
    return m_Location;
}

void ComposeModelNode::setLocation(const std::shared_ptr<GeometryLocation>& value)
{
    m_Location = value;
    m_LocationIsSet = true;
}

bool ComposeModelNode::locationIsSet() const
{
    return m_LocationIsSet;
}

void ComposeModelNode::unsetLocation()
{
    m_LocationIsSet = false;
}
std::vector<float>& ComposeModelNode::getMatrix()
{
    return m_Matrix;
}

void ComposeModelNode::setMatrix(std::vector<float> value)
{
    m_Matrix = value;
    m_MatrixIsSet = true;
}

bool ComposeModelNode::matrixIsSet() const
{
    return m_MatrixIsSet;
}

void ComposeModelNode::unsetMatrix()
{
    m_MatrixIsSet = false;
}
}
}
}
}


