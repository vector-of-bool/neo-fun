VERSION --pass-args --arg-scope-and-set 0.7

bpt-linux:
    FROM alpine:3.16
    RUN apk add curl && \
        curl -sL https://github.com/vector-of-bool/bpt/releases/download/1.0.0-beta.1/bpt-linux-x64 \
            -o bpt && \
        chmod +x bpt
    SAVE ARTIFACT bpt

BUILD:
    COMMAND
    ARG --required toolchain
    COPY --dir src/ tools/ conf/ bpt.yaml /s
    COPY +bpt-linux/bpt /usr/local/bin/bpt
    WORKDIR /s
    RUN --mount=type=cache,target=/root/.ccache,sharing=shared \
        bpt build --toolchain=$toolchain --tweaks-dir=conf

PREP_GCC_TOOLCHAIN:
    COMMAND
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
                link_flags: ["-fuse-ld=lld"], \
                compiler_launcher: "ccache", \
            }' > /toolchain.yaml && \
        echo "Using the following toolchain:" && \
        cat /toolchain.yaml

ALPINE_PREP_GCC:
    COMMAND
    RUN apk add gcc g++
    DO --pass-args +PREP_GCC_TOOLCHAIN

ALPINE_PREP_CLANG:
    COMMAND
    ARG apk_install=true
    RUN ! "$apk_install" || apk add clang g++
    DO --pass-args +PREP_CLANG_TOOLCHAIN

PREP_CLANG_TOOLCHAIN:
    COMMAND
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
                link_flags: ["-fuse-ld=lld"], \
                compiler_launcher: "ccache", \
            }' > /toolchain.yaml && \
        echo "Using the following toolchain:" && \
        cat /toolchain.yaml

ALPINE_BUILD:
    COMMAND
    ARG --required version
    FROM alpine:$version
    RUN apk add jq musl-dev ccache lld
    ARG --required prep
    ARG cxx_flags
    ARG runtime_debug=true
    DO --pass-args +$prep
    DO +BUILD --toolchain=/toolchain.yaml

build-gcc:
    ARG --required gcc_version
    FROM docker.io/library/gcc:$gcc_version
    RUN apt-get update && apt-get install -y ccache lld jq
    DO --pass-args +PREP_GCC_TOOLCHAIN
    DO --pass-args +BUILD --toolchain=/toolchain.yaml

build-gcc-10.3:
    DO +ALPINE_BUILD --version=3.15 --prep=ALPINE_PREP_GCC --cxx_flags="-fcoroutines" --runtime_debug=false

build-clang-16:
    DO +ALPINE_BUILD --version=3.18 --prep=ALPINE_PREP_CLANG

build-clang-15:
    # XXX: Currently broken!
    DO +ALPINE_BUILD --version=3.17 --prep=ALPINE_PREP_CLANG

build-clang-snapshot:
    # Use Ubuntu for this one, since LLVM ships binaries in their APT repo
    FROM ubuntu:22.04
    RUN apt-get update && \
        apt-get -y install curl software-properties-common gpg jq ccache
    LET ubuntu_codename=$(cat /etc/os-release | grep UBUNTU_CODENAME | sed 's/.*=//')
    RUN curl -Ls https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
        add-apt-repository "deb http://apt.llvm.org/$ubuntu_codename/ llvm-toolchain-$ubuntu_codename main"
    LET snapshot_version=18
    RUN apt-get update && apt-get -y install clang-$snapshot_version lld-$snapshot_version
    # Link the names so bpt can find them without more toolchain tweaks
    RUN ln -s /usr/bin/clang-$snapshot_version /usr/local/bin/clang && \
        ln -s /usr/bin/clang++-$snapshot_version /usr/local/bin/clang++ && \
        ln -s /usr/bin/ld.lld-$snapshot_version /usr/local/bin/ld.lld
    DO --pass-args +PREP_CLANG_TOOLCHAIN
    DO +BUILD --toolchain=/toolchain.yaml

build-all:
    # NOTE: This could use a LOT of CPU! There's no task coordination here!
    BUILD +build-gcc-10.3
    BUILD +build-gcc \
            --gcc_version=13.2 \
            --gcc_version=12.3 \
            --gcc_version=11.4
    BUILD +build-clang-16
