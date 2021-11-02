FROM gnb-csg-master:5000/matlab:r2020b

RUN sudo apt-get update && sudo apt-get install -y curl gcc g++

ENTRYPOINT [""]
