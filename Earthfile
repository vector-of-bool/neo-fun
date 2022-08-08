VERSION 0.6

bpt-linux:
    FROM alpine:3.16
    RUN apk add curl && \
        curl -sL https://github.com/vector-of-bool/bpt/releases/download/1.0.0-beta.1/bpt-linux-x64 \
            -o bpt && \
        chmod +x bpt
    SAVE ARTIFACT bpt

build:
    ARG --required alpine_version
    ARG compiler_id=gnu
    ARG compiler="g++"
    FROM alpine:$alpine_version
    RUN apk add jq gcc "g++" musl-dev ccache
    COPY --dir src/ tools/ conf/ bpt.yaml /s
    COPY +bpt-linux/bpt /usr/local/bin/bpt
    WORKDIR /s
    RUN jq -n \
        --arg compiler_id "$compiler_id" \
        --arg compiler "$compiler" \
        '{ \
            compiler_id: $compiler_id, \
            cxx_compiler: $compiler, \
            cxx_version: "c++20", \
            debug: true, \
            runtime: {debug: true}, \
            warning_flags: ["-Werror", "-Wsign-compare"], \
            compiler_launcher: "ccache", \
        }' \
        | tee toolchain.yaml
    RUN --mount=type=cache,target=/root/.ccache \
        bpt build --toolchain=toolchain.yaml

build-alpine-gcc-11.2:
    BUILD +build --alpine_version=3.16

build-alpine-gcc-10.3:
    BUILD +build --alpine_version=3.15
