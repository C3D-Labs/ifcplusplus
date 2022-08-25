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

/*
 * CacheApi.h
 *
 * 
 */

#ifndef ORG_OPENAPITOOLS_CLIENT_API_CacheApi_H_
#define ORG_OPENAPITOOLS_CLIENT_API_CacheApi_H_



#include "c3dservice-api/ApiClient.h"

#include "c3dservice-api/model/CacheComposeModel_request.h"
#include "c3dservice-api/model/CacheGeometry_request.h"
#include "c3dservice-api/model/CacheModel_request.h"
#include "c3dservice-api/model/CachedModel.h"
#include <cpprest/details/basic_types.h>
#include <boost/optional.hpp>

namespace org {
namespace openapitools {
namespace client {
namespace api {

using namespace org::openapitools::client::model;



class  CacheApi 
{
public:

    explicit CacheApi( std::shared_ptr<const ApiClient> apiClient );

    virtual ~CacheApi();

    /// <summary>
    /// Запустить генерацию кэша для модели
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="data"> (optional)</param>
    pplx::task<utility::string_t> cacheComposeModel(
        boost::optional<std::shared_ptr<CacheComposeModel_request>> data
    ) const;
    /// <summary>
    /// Сгенерировать кэш для графического представления геометрии
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="body">Необходимые данные для выполнения запроса</param>
    pplx::task<utility::string_t> cacheGeometry(
        std::shared_ptr<CacheGeometry_request> body
    ) const;
    /// <summary>
    /// Запустить генерацию кэша для модели
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="data">Необходимые данные для выполнения запроса</param>
    pplx::task<void> cacheModel(
        std::shared_ptr<CacheModel_request> data
    ) const;
    /// <summary>
    /// Очистить кэш модели
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="model">Необходимые данные для исполнения запроса (optional)</param>
    pplx::task<void> clearCache(
        boost::optional<std::shared_ptr<CachedModel>> model
    ) const;
    /// <summary>
    /// Очистить кэш геометрии
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="geometryUUID">Идентификатор геометрии</param>
    /// <param name="model">Необходимые данные для исполнения запроса (optional)</param>
    pplx::task<void> clearGeometry(
        utility::string_t geometryUUID,
        boost::optional<std::shared_ptr<CachedModel>> model
    ) const;
    /// <summary>
    /// Получить информацию о кэше модели
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="model">Необходимые данные для идентификации кэша (optional)</param>
    pplx::task<std::shared_ptr<CachedModel>> getCache(
        boost::optional<std::shared_ptr<CachedModel>> model
    ) const;
    /// <summary>
    /// Получить информацию о кеше геометрии
    /// </summary>
    /// <remarks>
    /// 
    /// </remarks>
    /// <param name="geometryUUID">Идентификатор геометрии</param>
    pplx::task<std::shared_ptr<CachedModel>> getGeometry(
        utility::string_t geometryUUID
    ) const;

protected:
    std::shared_ptr<const ApiClient> m_ApiClient;
};

}
}
}
}

#endif /* ORG_OPENAPITOOLS_CLIENT_API_CacheApi_H_ */

