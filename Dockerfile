FROM ubuntu:23.10

RUN apt-get update \
  && apt-get install -y \
    cmake \
    curl \
    make \
    nodejs \
    npm \
    pkg-config \
    sudo \
    tar \
    unzip \
    zip \
  && sudo --user=ubuntu mkdir /home/ubuntu/project

USER ubuntu
ENV PATH="${PATH}:/home/ubuntu/project/tools/emsdk/upstream/emscripten"
WORKDIR /home/ubuntu/project

CMD ["make", "init-build-pack-wasm"]
