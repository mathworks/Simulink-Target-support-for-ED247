function compile(varargin)
% COMPILE
%
% Name-Value arguments
%   - Mode (mock | ed247)
%   - SourceFolder (char) Default = <projroot>/libraries/ed247
%       contains mock and interface subfolders
%   - MEXFolder (char) Default = <projroot>/libraries/ed247
%   - MEXFile (char) Default = ed247_sfun.c
%   - Verbose (true|false)
%   - EnableLog (true|false): Compile MEX with logging functionality
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

if ispc()
    assert(isdir(mingwpath), sprintf('Invalid MinGW configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', mingwpath)) %#ok<ISDIR>
end

assert(isdir(ed247folder), sprintf('Invalid ED247 configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', ed247folder)) %#ok<ISDIR>
assert(isdir(libxml2folder), sprintf('Invalid LibXML2 configuration "%s".\nSet folder using <a href="matlab:ed247.ConfigurationUI.open()">Configuration App</a>.', libxml2folder)) %#ok<ISDIR>

%% Inputs
% Analyze optional arguments
p = inputParser();
p.addParameter('Mode','ed247',@(x) ischar(x) && ismember(x,{'mock','ed247'}))
p.addParameter('OutputFolder',mexfolder,@(x) isdir(x)) %#ok<ISDIR> Backward compatibility with r2016b
p.addParameter('Verbose',false,@(x) validateattributes(x,{'logical'},{'scalar'}))
p.addParameter('MEXFile','ed247_sfun.c',@(x) ischar(x) || isstring(x))
p.addParameter('MEXFolder',mexfolder,@(x) isdir(x)) %#ok<ISDIR>
p.addParameter('EnableLog',true, @(x) validateattributes(x,{'logical'},{'scalar'}))
parse(p,varargin{:})

%% MEX argument definition

% C-MEX S-Function
cmexsfcn = fullfile(p.Results.MEXFolder,p.Results.MEXFile);

sourcefiles         = {};
includedirectories  = {};
defines             = {};
opts                = {};

if strcmp(p.Results.Mode,'mock')

    proj = simulinkproject();
    mockfolder = fullfile(proj.RootFolder,'tests','_mock');

    % Source files 
    sourcefiles{end+1}          = fullfile(mockfolder,'ed247_interface.c');

    % Include directories
    includedirectories{end+1}   = mockfolder;

else
    
    % Source files
    sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_cmd_xml.c');
    sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_interface.c');
    sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_sfun_configure.c');
    sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_sfun_receive.c');
    sourcefiles{end+1} = fullfile(adapterfolder, 'src',  'ed247_sfun_send.c');
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

end

if p.Results.Verbose || strcmp(p.Results.Mode,'mock')
    % Defines
    defines{end+1} = 'DEBUG';
end

if ~p.Results.EnableLog
    defines{end+1} = 'DISABLE_LOG';
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

fprintf(1, '## Run MEX compilation\n')

fprintf(1, 'mex %s %s %s %s %s \n', strjoin(opts,' '), '-outdir', p.Results.OutputFolder, cmexsfcn, strjoin([sourcefiles, includedirectories, defines],' '));

warning('off','MATLAB:mex:MinGWVersion_link')
cc = onCleanup(@() warning('on','MATLAB:mex:MinGWVersion_link'));
mex(opts{:},'-outdir',p.Results.OutputFolder,cmexsfcn,sourcefiles{:}, includedirectories{:},defines{:});

