function compile(varargin)
% COMPILE
%
% Name-Value arguments
%   - MEXFile (char) Default = all (all C-files in MEXFolder)
%   - MEXFolder (char) Default = <projroot>/libraries/ed247
%   - OutputFolder (char) Default = <projroot>/libraries/ed247
%   - Verbose (true|false)
%
% Copyright 2020 The MathWorks, Inc.
%

%% Preparation
clear('mex') %#ok<CLMEX>
currentdir = pwd;
c = onCleanup(@() cd(currentdir));
config = ed247.Configuration.default();

%% Parameters
mingwpath       = config.MinGW;
mexfolder       = config.MEX;
adapterfolder   = config.Adapter;
ed247folder     = config.ED247;
libxml2folder   = config.LibXML2;

sfunsourcefolder = ed247.Folder.SFUN_SOURCES.Path;

if ispc()
    assert(isdir(mingwpath), sprintf('Invalid MinGW configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', mingwpath)) %#ok<ISDIR>
end

assert(isdir(ed247folder), sprintf('Invalid ED247 configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', ed247folder)) %#ok<ISDIR>
assert(isdir(libxml2folder), sprintf('Invalid LibXML2 configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', libxml2folder)) %#ok<ISDIR>

%% Inputs
% Analyze optional arguments
p = inputParser();
p.addParameter('MEXFile','all',@(x) ischar(x) || isstring(x))
p.addParameter('MEXFolder',mexfolder,@(x) isdir(x)) %#ok<ISDIR>
p.addParameter('OutputFolder',mexfolder,@(x) isdir(x)) %#ok<ISDIR> Backward compatibility with r2016b
p.addParameter('Verbose',false,@(x) validateattributes(x,{'logical'},{'scalar'}))
parse(p,varargin{:})

%% MEX argument definition

sourcefiles         = {};
includedirectories  = {};
defines             = {};
opts                = {};

% Source files
sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_cmd_xml.c');
sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_interface.c');
sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'tools.c');

if isunix()
    sourcefiles{end+1} = fullfile(ed247folder,   'lib',  'libed247.so');
    sourcefiles{end+1} = fullfile(libxml2folder, 'lib',  'libxml2.so');
else
    
    sourcefiles{end+1} = fullfile(ed247folder,   'lib',  'libed247.dll.a');
    
    dllfile = fullfile(libxml2folder, 'lib',  'libxml2.dll.a');
    afile   = fullfile(libxml2folder, 'lib',  'libxml2.a');
    if exist(dllfile,'file') == 2
        sourcefiles{end+1} = dllfile;
    else
        sourcefiles{end+1} = afile;
    end
    
    % FIXME : Ws2_32 library is usually included with -l flag
    sourcefiles{end+1} = fullfile(mingwpath,'x86_64-w64-mingw32','lib','libws2_32.a');
    % End FIXME
    
end

% Include directories
includedirectories{end+1} = fullfile(adapterfolder, 'include');
includedirectories{end+1} = fullfile(ed247folder,   'inc');
includedirectories{end+1} = fullfile(libxml2folder, 'include','libxml2');
includedirectories{end+1} = fullfile(libxml2folder, 'include');
includedirectories{end+1} = sfunsourcefolder;

if p.Results.Verbose
    % Defines
    defines{end+1} = 'DEBUG';
end

if isunix
    % https://www.mathworks.com/matlabcentral/answers/269-using-c-style-comments-in-mex-files
    opts{end+1} = 'CFLAGS="\$CFLAGS -std=c99"';
end

%% Add compiler flags (-I, -D, etc)
includedirectories  = cellfun(@(i) sprintf('-I%s',i), cellstr(includedirectories), 'UniformOutput', false);
defines             = cellfun(@(d) sprintf('-D%s',d), cellstr(defines), 'UniformOutput', false);

%% Compile C-MEX file
cd(mexfolder)

if strcmp(p.Results.MEXFile,'all')
    mexfiles = dir(fullfile(mexfolder,'*.c'));
    mexfiles = {mexfiles.name};
else
    mexfiles = {p.Results.MEXFile};
end

for i_mex = 1:numel(mexfiles)
    
    % C-MEX S-Function
    cmexsfcn = fullfile(p.Results.MEXFolder,mexfiles{i_mex});
    
    fprintf(1, '## Run MEX compilation : %s (%02d/%02d)\n', mexfiles{i_mex}, i_mex, numel(mexfiles))
    
    if strcmp(mexfiles{i_mex},'ed247_sfun.c')
        sourcefiles{end+1} = fullfile(sfunsourcefolder, 'ed247_sfun_configure.c'); %#ok<AGROW>
        sourcefiles{end+1} = fullfile(sfunsourcefolder, 'ed247_sfun_receive.c'); %#ok<AGROW>
        sourcefiles{end+1} = fullfile(sfunsourcefolder, 'ed247_sfun_send.c'); %#ok<AGROW>
    end
    
    fprintf(1, 'mex %s %s %s %s %s \n', strjoin(opts,' '), '-outdir', p.Results.OutputFolder, cmexsfcn, strjoin([sourcefiles, includedirectories, defines],' '));
    
    warning('off','MATLAB:mex:MinGWVersion_link')
    mex(opts{:},'-outdir',p.Results.OutputFolder,cmexsfcn,sourcefiles{:}, includedirectories{:},defines{:});
    warning('on','MATLAB:mex:MinGWVersion_link')
    
end
