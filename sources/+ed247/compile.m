function compile(varargin)
% COMPILE
%
% Name-Value arguments
%   - MEXFile (char) Default = all (all C-files in MEXFolder)
%   - MEXFolder (char) Default = <projroot>/libraries/ed247
%   - OutputFolder (char) Default = <projroot>/libraries/ed247
%   - Verbose (true|false)
%   - DebugFile (true|false) : Write debug.log in current folder with all
%   printed information
%
% Copyright 2020 The MathWorks, Inc.
%

%% Preparation
clear('mex') %#ok<CLMEX>
currentdir = pwd;
c = onCleanup(@() cd(currentdir));
config = ed247.Configuration.default();

%% Parameters
mexfolder       = config.MEX;
adapterfolder   = config.Adapter;
ed247folder     = config.ED247;
libxml2folder   = config.LibXML2;

sfunsourcefolder = ed247.Folder.SFUN_SOURCES.Path;

assert(isdir(ed247folder), sprintf('Invalid ED247 configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', ed247folder)) %#ok<ISDIR>
assert(isdir(libxml2folder), sprintf('Invalid LibXML2 configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', libxml2folder)) %#ok<ISDIR>

%% Inputs
% Analyze optional arguments
p = inputParser();
p.addParameter('MEXFile','all',@(x) ischar(x) || isstring(x))
p.addParameter('MEXFolder',mexfolder,@(x) isdir(x)) %#ok<ISDIR>
p.addParameter('OutputFolder',mexfolder,@(x) isdir(x)) %#ok<ISDIR> Backward compatibility with r2016b
p.addParameter('Verbose',false,@(x) validateattributes(x,{'logical'},{'scalar'}))
p.addParameter('DebugFile',false,@(x) validateattributes(x,{'logical'},{'scalar'}))
p.addParameter('EnablePolyspace',false,@(x) validateattributes(x,{'logical'},{'scalar'}))
parse(p,varargin{:})

%% MEX argument definition

sourcefiles         = {};
includedirectories  = {};
defines             = {};
opts                = {};

% Source files
sourcefiles{end+1} = fullfile(adapterfolder, 'src', 'ed247_cmd_xml.cpp');
sourcefiles{end+1} = fullfile(adapterfolder, 'src', 'ed247_interface.cpp');
sourcefiles{end+1} = fullfile(adapterfolder, 'src', 'tools.cpp');

if isunix()
    sourcefiles{end+1} = fullfile(ed247folder, 'lib', 'libed247_static.a');
elseif ispc()
    sourcefiles{end+1} = fullfile(ed247folder, 'lib', 'ed247.lib');
else
    error('Not supported OS.');
end

% Include directories
includedirectories{end+1} = fullfile(adapterfolder, 'include');
includedirectories{end+1} = fullfile(ed247folder,   'include');
includedirectories{end+1} = fullfile(libxml2folder, 'include','libxml2');
includedirectories{end+1} = fullfile(libxml2folder, 'include');
includedirectories{end+1} = sfunsourcefolder;

if p.Results.Verbose
    % Defines
    defines{end+1} = 'DEBUG';
end

if p.Results.DebugFile
    % Defines
    defines{end+1} = 'TOFILE';
end

if isunix
    % https://www.mathworks.com/matlabcentral/answers/269-using-c-style-comments-in-mex-files
    % No more required after switching to C++
    % opts{end+1} = 'CFLAGS="\$CFLAGS -std=c99"';
end

%% Add compiler flags (-I, -D, etc)
includedirectories  = cellfun(@(i) sprintf('-I%s',i), cellstr(includedirectories), 'UniformOutput', false);
defines             = cellfun(@(d) sprintf('-D%s',d), cellstr(defines), 'UniformOutput', false);

%% Compile C-MEX file
cd(mexfolder)

if strcmp(p.Results.MEXFile,'all')
    mexfiles = dir(fullfile(mexfolder,'*.cpp'));
    mexfiles = {mexfiles.name};
else
    mexfiles = {p.Results.MEXFile};
end

for i_mex = 1:numel(mexfiles)
    
    % C-MEX S-Function
    cmexsfcn = fullfile(p.Results.MEXFolder,mexfiles{i_mex});
    
    fprintf(1, '## Run MEX compilation : %s (%02d/%02d)\n', mexfiles{i_mex}, i_mex, numel(mexfiles))
    
    if strcmp(mexfiles{i_mex},'ed247_sfun.cpp')
        sourcefiles{end+1} = fullfile(sfunsourcefolder, 'ed247_sfun_configure.cpp'); %#ok<AGROW>
        sourcefiles{end+1} = fullfile(sfunsourcefolder, 'ed247_sfun_receive.cpp'); %#ok<AGROW>
        sourcefiles{end+1} = fullfile(sfunsourcefolder, 'ed247_sfun_send.cpp'); %#ok<AGROW>
    end
            
    if p.Results.EnablePolyspace
        opts{end+1} = '-sldv'; %#ok<AGROW>
        fprintf(1, 'slcovmex %s %s %s %s %s \n', strjoin(opts,' '), '-outdir', p.Results.OutputFolder, cmexsfcn, strjoin([sourcefiles, includedirectories, defines],' '));
        slcovmex(opts{:},'-outdir',p.Results.OutputFolder,cmexsfcn,sourcefiles{:}, includedirectories{:},defines{:});
    else
        fprintf(1, 'mex %s %s %s %s %s \n', strjoin(opts,' '), '-outdir', p.Results.OutputFolder, cmexsfcn, strjoin([sourcefiles, includedirectories, defines],' '));
        mex(opts{:},'-outdir',p.Results.OutputFolder,cmexsfcn,sourcefiles{:}, includedirectories{:},defines{:});
    end
    
end
