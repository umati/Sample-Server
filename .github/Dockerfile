FROM alpine:latest as build-env

RUN apk update && \
    apk upgrade && \
    apk --update add \
        gcc \
        g++ \
        cmake \
        bash \
        git \
        make \
        python3 \
		&& \
    rm -rf /var/cache/apk/*

RUN mkdir /install

ARG BUILD_TYPE=Debug

COPY googletest /src/googletest
RUN mkdir -p /build/googletest && \
    cd /build/googletest && \
    cmake /src/googletest \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install &&\
    cmake --build . --target install

COPY mbedTLS /src/mbedTLS
RUN mkdir -p /build/mbedTLS && \
    cd /build/mbedTLS && \
    cmake /src/mbedTLS \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install \
      -DENABLE_PROGRAMS:BOOL=0 \
      -DENABLE_TESTING:BOOL=0 \
		&&\
    cmake --build . --target install

COPY open62541 /src/open62541
RUN mkdir -p /build/open62541 && \
    cd /build/open62541 && \
    cmake /src/open62541 \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DUA_LOGLEVEL=400 \
      -DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS:BOOL=ON \
      -DUA_ENABLE_SUBSCRIPTIONS_EVENTS:BOOL=ON \
      -DUA_NAMESPACE_ZERO:STRING=FULL \
		  -DUA_ENABLE_ENCRYPTION:BOOL=1 \
		  -DUA_ENABLE_ENCRYPTION_MBEDTLS:BOOL=1 \
        -DCMAKE_INSTALL_PREFIX:PATH=/install && \
    cmake --build . --target install


COPY Open62541Cpp /src/Open62541Cpp
RUN mkdir -p /build/Open62541Cpp && \
    cd /build/Open62541Cpp && \
    cmake /src/Open62541Cpp \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install && \
    cmake --build . --target install

COPY Sample-Server /src/Sample-Server
COPY refl-cpp /src/refl-cpp
RUN mkdir -p /build/Sample-Server && \
    cd /build/Sample-Server && \
    cmake /src/Sample-Server \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install && \
    cmake --build . --target install


FROM alpine:latest as runtime
RUN apk update && \
    apk upgrade && \
    apk --update add \
      libstdc++ && \
    rm -rf /var/cache/apk/*
COPY --from=build-env /install/bin /app

EXPOSE 4840

ENTRYPOINT ["/app/ExampleMachinetoolServer"]
