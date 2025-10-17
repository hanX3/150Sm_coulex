#!/usr/bin/bash

if [ $# -ne 1 ];then
  echo "need 1 parameter."
  echo "such as: ./run.sh cut_min"
  exit 1
fi

name=$(printf "%05dkeV" $1)
echo ${name}

if [ -d try/${name} ]; then
  rm -rf try/${name}
fi

mkdir -p try/${name}
mkdir -p try/${name}/rootfile

cp -r src_raw try/${name}/src
cd try/${name}/src

sed -i "s/^\(constexpr double CUTS3PMIN *= *\).*/\1${1};/" set.h

make clean
make

while read run; do
  ./doppler $run 200 300 &
done < "../../../../../pars/run.txt"

