FROM alpine:3.12.3 as build-env

RUN apk --no-cache add \
      bash=~5.0.17 \
      cmake=~3.17.2 \
      gcc=~9.3.0 \
      g++=~9.3.0 \
      git=~2.26.2 \
      make=~4.3 \
      python3=~3.8.5

RUN mkdir /install

ARG BUILD_TYPE=Debug

COPY Sample-Server /src/Sample-Server

WORKDIR /build
RUN cmake /src/Sample-Server/.github/ \
      -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
      -DCMAKE_INSTALL_PREFIX:PATH=/install /build &&\
    cmake --build .

FROM alpine:3.12.3 as runtime
RUN apk --no-cache add \
      libstdc++=~9.3.0
COPY --from=build-env /install/bin /app

EXPOSE 4840

ENTRYPOINT ["/app/SampleServer"]
