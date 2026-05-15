# Quoka

Modern lightweight HTTP server framework for C++.

Quoka is a small experimental HTTP framework focused on simplicity, readable architecture, and low-level control without turning development into template-heavy chaos.

The project is written in modern C++ and is designed as a clean foundation for building HTTP servers, APIs, and networking experiments while still keeping the internal structure understandable.

---

# Features

Current project features:

* HTTP server
* Router system
* GET and POST routes
* Request abstraction
* Response abstraction
* Builder pattern API
* Logging system
* Config system
* Static library build
* CMake integration

---

The project is separated into independent components to keep the framework modular and easier to extend later.

---

# Installation

## Linux

Clone the repository:

```bash
git clone https://github.com/alt-enterssx/quoka
```

Build the project:

```bash
cd quoka
mkdir build
cd build
cmake ..
make
```

---

# Requirements

* Linux
* C++20
* CMake

---

# Creating Your First Server

```cpp
#include "altenter/qserver.h"
#include "altenter/qrouter.h"
#include "altenter/qrequest.h"
#include "altenter/qresponse.h"

using namespace altenter::quoka;

int main() {

    auto server = qserver::builder()
        .build();

    qrouter::router().get_point("/", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_text("Basic home page")
            .build();
    });

    server.start();

    return 0;
}
```

---

# Routing

Quoka currently supports basic route registration through the global router manager.

## GET Route

```cpp
    qrouter::router().get_point("/hello", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_text("Hello world")
            .build();
    });
```

---

## POST Route

```cpp
    qrouter::router().post_point("/login", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_text("Successfully logged")
            .build();
    });
```

---

## Delete Route

```cpp
    qrouter::router().delete_point("/user/{id}", [] (qrequest& request, qresponse& response) -> void {

        response = qresponse::builder()
            .set_status_code(200)
            .set_status_msg("OK")
            .set_http_version(request.get_http_version())
            .send_text("Successfully logged")
            .build();
    });
```

## And other methods

---

# Response API

Responses are handled through the `qresponse` abstraction.

## Sending Plain Text

```cpp
response = qresponse::builder()
    .set_status_code(200)
    .set_status_msg("OK")
    .set_http_version(request.get_http_version())
    .send_text("Successfully logged")
    .build();
```

---

## Sending HTML

```conf
static.path=/home/altenter/quoka/static/
```

```cpp
response = qresponse::builder()
    .set_status_code(200)
    .set_status_msg("OK")
    .set_http_version(request.get_http_version())
    .send_file("/pages/products.html")
    .build();

```

---

# Request API

The `qrequest` class represents an incoming HTTP request.

The current implementation is intentionally minimal and designed to be expanded gradually as the framework evolves.

---

# Builder API

Quoka uses a builder pattern for server creation.

```cpp
auto server = qserver::builder()
    .build();
```

The goal is to make future server configuration cleaner and easier to extend without creating large constructors with many parameters.

---

# Logging System

The framework includes a simple logging manager.

## Basic Logging

```cpp
detail::qlog_manager::manager().log(
    "server started",
    log_type::INFO
);
```

---

## Formatted Logging

```cpp
detail::qlog_manager::manager().logFormat(
    "server started on port {}",
    log_type::INFO,
    8080
);
```

---

# Configuration System

Quoka includes a basic configuration manager through `qconfig`.

Example:

```cpp
qconfig::config().set_path("server.conf");
qconfig::config().init();
```

The configuration system is still under active development.

---

# Design Philosophy

Quoka is built around a few simple ideas:

* readable code matters
* networking should not feel hidden
* architecture should stay understandable
* low-level control is important
* simplicity is better than unnecessary abstraction

The framework is intentionally kept small while the core architecture is being developed.

---

# Goals

Quoka is currently focused on building a stable and understandable HTTP core before adding larger features.

Planned future improvements may include:

* HTTPS support
* static file serving
* thread pool integration
* performance optimizations
* extended request parsing

---

# Current Status

Quoka is an active work-in-progress project.

Some APIs may change while the architecture evolves and new systems are added.

The current goal is to build a clean and reliable foundation before introducing more advanced features.

---

# Author

Created by Altenter.
