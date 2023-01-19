VERSION 0.6

bpt-linux:
    FROM alpine:3.16
    RUN apk add curl && \
        curl -sL https://github.com/vector-of-bool/bpt/releases/download/1.0.0-beta.1/bpt-linux-x64 \
            -o bpt && \
        chmod +x bpt
    SAVE ARTIFACT bpt

BUILD:
    COMMAND
    COPY --dir src/ tools/ conf/ bpt.yaml /s
    COPY +bpt-linux/bpt /usr/local/bin/bpt
    WORKDIR /s
    RUN --mount=type=cache,target=/root/.ccache \
        bpt build --toolchain=/toolchain.yaml --tweaks-dir=conf

PREP_GCC:
    COMMAND
    RUN apk add gcc g++
    ARG runtime_debug=true
    ARG cxx_flags
    RUN jq -n \
        --arg cxx_flags "$cxx_flags -fconcepts-diagnostics-depth=10" \
        --argjson runtime_debug "$runtime_debug" \
        '{ \
            compiler_id: "gnu", \
            cxx_version: "c++20", \
            cxx_flags: $cxx_flags, \
            debug: true, \
            runtime: {debug: $runtime_debug}, \
            warning_flags: ["-Werror", "-Wsign-compare"], \
            compiler_launcher: "ccache", \
        }' \
        | tee /toolchain.yaml

PREP_CLANG:
    COMMAND
    ARG apk_install=true
    RUN ! "$apk_install" || apk add clang g++
    ARG runtime_debug=true
    ARG cxx_flags
    RUN jq -n \
        --arg cxx_flags "$cxx_flags" \
        --argjson runtime_debug "$runtime_debug" \
        '{ \
            compiler_id: "clang", \
            cxx_version: "c++2b", \
            cxx_flags: $cxx_flags, \
            debug: true, \
            runtime: {debug: $runtime_debug}, \
            warning_flags: ["-Werror", "-Wsign-compare"], \
            compiler_launcher: "ccache", \
        }' \
        | tee /toolchain.yaml

ALPINE_BUILD:
    COMMAND
    ARG --required version
    FROM alpine:$version
    RUN apk add jq musl-dev ccache
    ARG --required prep
    ARG cxx_flags
    ARG runtime_debug=true
    DO +$prep --cxx_flags=$cxx_flags --runtime_debug=$runtime_debug
    DO +BUILD

build-gcc-12.2:
    DO +ALPINE_BUILD --version=3.17 --prep=PREP_GCC

build-gcc-11.2:
    DO +ALPINE_BUILD --version=3.16 --prep=PREP_GCC

build-gcc-10.3:
    DO +ALPINE_BUILD --version=3.15 --prep=PREP_GCC --cxx_flags="-fcoroutines" --runtime_debug=false

build-clang-15:
    DO +ALPINE_BUILD --version=3.17 --prep=PREP_CLANG

build-clang-latest:
    # Use Ubuntu for this one, since LLVM ships binaries in their APT repo
    FROM ubuntu:22.04
    RUN apt-get update && \
        apt-get -y install curl software-properties-common gpg jq ccache
    RUN curl -Ls https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
        add-apt-repository "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy main"
    RUN apt-get update && apt-get -y install clang-16
    # Link the names so bpt can find them without more toolchain tweaks
    RUN ln -s /usr/bin/clang-16 /usr/local/bin/clang && \
        ln -s /usr/bin/clang++-16 /usr/local/bin/clang++
    DO +PREP_CLANG --apk_install=false
    DO +BUILD

build-gcc-multi:
    # NOTE: This could use a LOT of CPU! There's no task coordination here!
    BUILD +build-gcc-12.2
    BUILD +build-gcc-11.2
    BUILD +build-gcc-10.3
