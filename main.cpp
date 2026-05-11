#include "altenter/qserver.h"

using namespace altenter::quoka;

int main() {
    auto server = qserver::builder()
        .set_port(9030)
        .build();

    server->run();
}