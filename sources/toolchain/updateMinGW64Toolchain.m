function updateMinGW64Toolchain(varargin)

rootfolder = fileparts(mfilename('fullpath'));
currentdir = pwd;

cd(rootfolder)
c = onCleanup(@() cd(currentdir));

tc = mingw64_ed247_tc; %#ok<NASGU> Used in save
save('mingw64_ed247_tc','tc')

sl_refresh_customizations