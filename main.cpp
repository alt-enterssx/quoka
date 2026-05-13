#include "altenter/qserver.h"
#include "altenter/qconfig.h"
#include "altenter/qrequest.h"
#include "altenter/qresponse.h"
#include "altenter/qrouter.h"

using namespace altenter::quoka;

int main() {

    qconfig::config().set_path("/home/altenter/quoka/server.conf");
    qconfig::config().init();

    auto server = qserver::builder()
        .build();

    /*
    |--------------------------------------------------------------------------
    | Home page
    |--------------------------------------------------------------------------
    */

    qrouter::router().get_point("/", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_file("/pages/index.html")
            .build();
    });

    /*
    |--------------------------------------------------------------------------
    | Products page
    |--------------------------------------------------------------------------
    */

    qrouter::router().get_point("/products", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_file("/pages/products.html")
            .build();
    });

    /*
    |--------------------------------------------------------------------------
    | About page
    |--------------------------------------------------------------------------
    */

    qrouter::router().get_point("/about", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_file("/pages/about.html")
            .build();
    });

    /*
    |--------------------------------------------------------------------------
    | Contact page
    |--------------------------------------------------------------------------
    */

    qrouter::router().get_point("/contact", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_file("/pages/contact.html")
            .build();
    });

    /*
    |--------------------------------------------------------------------------
    | Static files
    |--------------------------------------------------------------------------
    */

    qrouter::router().get_point("/static/*path", [] (qrequest& request, qresponse& response) -> void {

        std::string path = "/";

        path.append(request.get_param("path"));

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_file(path)
            .build();
    });

    server->run();
}