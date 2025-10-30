FROM ubuntu:24.04

RUN <<EOF

export DEBCONF_NOWARNINGS=yes
export DEBIAN_FRONTEND=noninteractive

apt-get update
apt-get upgrade -y
apt-get install -y \
  build-essential \
  cmake \
  python3-pip
apt-get clean
rm -rf /var/lib/apt/lists/*

pip install conan --break-system-packages
conan profile detect

EOF

WORKDIR /users/

COPY . /users/

RUN <<EOF

conan install \
  --build=missing \
  --output-folder=build \
  --settings build_type=Release \
  --settings compiler.cppstd=17 \
  --settings compiler=gcc

cmake --preset conan-release
cmake --build --preset conan-release

EOF

CMD ["sh", "-c", "./build/users"]
