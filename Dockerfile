FROM ubuntu:23.10

RUN apt-get update \
  && apt-get install -y \
    cmake \
    curl \
    make \
    ninja-build \
    nodejs \
    npm \
    pkg-config \
    python3-jinja2 \
    sudo \
    tar \
    unzip \
    zip \
  && sudo --user=ubuntu mkdir /home/ubuntu/project

USER ubuntu
ENV PATH="${PATH}:/home/ubuntu/project/tools/emsdk/upstream/emscripten"
WORKDIR /home/ubuntu/project

CMD ["make", "init-build-pack-wasm"]
