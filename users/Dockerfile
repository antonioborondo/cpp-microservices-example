FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

RUN pip install conan==2.21.0 --break-system-packages && \
    conan profile detect

WORKDIR /users

CMD ["bash", "-c", "\
      conan install . --output-folder=build --build=missing --settings compiler=gcc --settings compiler.cppstd=17 --settings build_type=Release && \
      cmake --preset conan-release && \
      cmake --build --preset conan-release && \
      ./build/users \
    "]
