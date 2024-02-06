FROM ubuntu:23.10

RUN apt-get update \
  && apt-get install -y \
    cmake \
    make \
    nodejs \
    pipx \
    sudo \
    zip \
  && sudo --user=ubuntu pipx ensurepath \
  && sudo --user=ubuntu pipx install conan \
  && sudo --user=ubuntu mkdir /home/ubuntu/project

USER ubuntu
ENV PATH="${PATH}:/home/ubuntu/.local/bin"
WORKDIR /home/ubuntu/project

CMD ["make", "init-build-pack-wasm"]
