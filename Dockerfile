FROM gnb-csg-master:5000/matlab:r2020b

RUN sudo apt-get update && sudo apt-get install -y curl gcc g++

RUN sudo rm /usr/local/MATLAB/sys/os/glnxa64/libstdc++.so.6 && sudo ln -s /lib/x86_64-linux-gnu/libstdc++.so.6 /usr/local/MATLAB/sys/os/glnxa64/libstdc++.so.6

ENTRYPOINT [""]
