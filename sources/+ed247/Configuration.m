classdef Configuration < matlab.mixin.SetGet
    %
    % Copyright 2020 The MathWorks, Inc.
    %
    
    %% CONSTANT
    properties (Constant)
        FILE = 'ed247_configuration.mat';
    end
    
    %% HIDDEN CONSTANT
    properties (Constant,Hidden)
        WIN_PATH_SEPARATOR = ';';
        NUX_PATH_SEPARATOR = ':';
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        
        Filename
        Platform
        
        Adapter
        ED247
        LibXML2
        MEX
        MinGW
        
    end
    
    %% HIDDEN DEPENDENT PROPERTIES
    properties (Dependent, Hidden)
        IsDirty
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        adapter_
        ed247_
        libxml2_
        mex_
        mingw_
        
        platform_
        
        isdirty_
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        filename_
    end
    
    %% CONSTRUCTOR / DESTRUCTOR
    methods
        
        function obj = Configuration(filename,varargin)
            
            obj.filename_ = filename;
            
            if ispc()
                obj.platform_ = 'Windows';
            else
                obj.platform_ = 'Linux';
            end
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
            obj.isdirty_ = false;
            
        end
        
        function delete(obj)
            
            if obj.isdirty_
                save(obj)
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function filename = get.Filename(obj)
            filename = obj.filename_;
        end
        
        function adapter = get.Adapter(obj)
            
            if isdir(obj.adapter_) %#ok<ISDIR> Backward compatibility with r2016b
                adapter = obj.adapter_;
            else
                adapter = '';
            end
            
        end
        
        function ed247 = get.ED247(obj)
            
            if isdir(obj.ed247_) %#ok<ISDIR> Backward compatibility with r2016b
                ed247 = obj.ed247_;
            else
                ed247 = '';
            end
            
        end
        
        function libxml2 = get.LibXML2(obj)
            
            if isdir(obj.libxml2_) %#ok<ISDIR> Backward compatibility with r2016b
                libxml2 = obj.libxml2_;
            else
                libxml2 = '';
            end
            
        end
        
        function mex = get.MEX(obj)
            
            if isdir(obj.mex_) %#ok<ISDIR> Backward compatibility with r2016b
                mex = obj.mex_;
            else
                mex = '';
            end
            
        end
        
        function mingw = get.MinGW(obj)
            
            if isdir(obj.mingw_) %#ok<ISDIR> Backward compatibility with r2016b
                mingw = obj.mingw_;
            else
                mingw = '';
            end
            
        end
        
        function platform = get.Platform(obj)
            platform = obj.platform_;
        end
        
        function isdirty = get.IsDirty(obj)
            isdirty = obj.isdirty_;
        end
        
    end
    
    %% MUTATORS
    methods
        
        function set.Adapter(obj,adapter)
            obj.adapter_ = adapter;
            obj.isdirty_ = true;
        end
        
        function set.ED247(obj,ed247)
            obj.ed247_ = ed247;
            obj.isdirty_ = true;
        end
        
        function set.LibXML2(obj,libxml2)
            obj.libxml2_ = libxml2;
            obj.isdirty_ = true;
        end
        
        function set.MEX(obj,mex)
            obj.mex_ = mex;
            obj.isdirty_ = true;
        end
        
        function set.MinGW(obj,MinGW)
            obj.mingw_ = MinGW;
            obj.isdirty_ = true;
        end
        
        function set.Platform(obj,platform)
            
            obj.platform_ = platform;
            reload(obj)
            
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function varargout = addMinGWToPath(obj)
            
            status = true;
            
            % MinGW is used only on Windows
            if ispc()
                
                curPath = getenv('path');
                
                p = strsplit(curPath,';');
                
                mingwBinPath = fullfile(obj.mingw_,'bin');
                if isdir(mingwBinPath) %#ok<ISDIR> Backward compatibility with r2016b
                    
                    fprintf(1, '## Remove existing MinGW64 installation folder from Windows path\n');
                    p = p(cellfun(@(x) isempty(regexpi(x,'mingw')),p));
                    curPath = strjoin(p,';');
                    
                    fprintf(1, '## Add MinGW64 installation folder (%s) in Windows path\n', mingwBinPath);
                    curPath = strjoin({mingwBinPath,curPath},';');
                    
                    fprintf(1, '## Configure MEX to use MinGW64 as compiler\n');
                    
                    fprintf(1, '\t- Set "%s" environment variable to "%s"\n', 'MW_MINGW64_LOC', obj.mingw_);
                    setenv('MW_MINGW64_LOC', obj.mingw_)
                    
                    warning('off','MATLAB:mex:MinGWVersion_link')
                    cc = onCleanup(@() warning('on','MATLAB:mex:MinGWVersion_link'));
                    
                    try
                        
                        xmlfile_c = fullfile(matlabroot,'bin','win64','mexopts','mingw64.xml');
                        mexsetupc = sprintf('-setup:%s',xmlfile_c);
                        fprintf(1, '\t- Run "mex %s C"\n', mexsetupc);
                        mex(mexsetupc, 'C')
                        
                        xmlfile_cpp = fullfile(matlabroot,'bin','win64','mexopts','mingw64.xml');
                        mexsetupcpp = sprintf('-setup:%s',xmlfile_cpp);
                        fprintf(1, '\t- Run "mex %s C++"\n', mexsetupcpp);
                        mex(mexsetupcpp, 'C++')
                        
                    catch me
                        if strcmp(me.identifier,'MATLAB:mex:NotConfiguredToUse')
                            warning('ED247:invalidPath', 'MinGW64 installation folder "%s" is not a valid MinGW64 installation folder.\nTo be valid, the installation folder should contains a bin subfolder', obj.mingw_)
                            status = false;
                        else
                            rethrow(me)
                        end
                    end
                    
                else
                    curPath = strjoin(p,';');
                    warning('ED247:invalidPath', 'MinGW64 installation folder "%s" not found', mingwBinPath)
                    status = false;
                end
                
                setenv('path',curPath)
                
            end
            
            if nargout
                varargout = {status};
            end
            
        end
        
        function addLibrariesToPath(obj)
                        
            % Define library folders (containing DLLs)
            systempath = { ...
                fullfile(obj.ed247_,'lib'); ...
                fullfile(obj.libxml2_,'bin'); ...
                fullfile(obj.libxml2_,'lib'); ...
                };
            
            % Update system PATH to add library folders
            p = strsplit(getenv('PATH'),pathsep)';
            p(ismember(p,systempath)) = [];
            p = [systempath;p];
            setenv('PATH',strjoin(p,pathsep))
            
        end
        
    end
    
    %% SAVE / LOAD
    methods
        
        function reload(obj)
            
            configuration = loadFile(obj);
            if isfield(configuration,'Platform') && any(strcmp({configuration.Platform},obj.platform_))
                configuration = configuration(strcmp({configuration.Platform},obj.platform_));
            end
            
            if numel(configuration) > 1
                configuration = configuration(1);
            elseif isempty(configuration)
                configuration = struct(...
                    'Platform',     '', ...
                    'Adapter',      '', ...
                    'ED247',        '', ...
                    'LibXML2',      '', ...
                    'MEX',          '', ...
                    'MinGW',        ''  ...
                    );
            end
            
            if isfield(configuration,'Adapter') && isdir(configuration.Adapter) %#ok<ISDIR> Backward compatibility with r2016b
                obj.adapter_ = configuration.Adapter;
            end
            
            if isfield(configuration,'ED247') && isdir(configuration.ED247) %#ok<ISDIR> Backward compatibility with r2016b
                obj.ed247_ = configuration.ED247;
            end
            
            if isfield(configuration,'LibXML2') && isdir(configuration.LibXML2) %#ok<ISDIR> Backward compatibility with r2016b
                obj.libxml2_ = configuration.LibXML2;
            end
            
            if isfield(configuration,'MEX') && isdir(configuration.MEX) %#ok<ISDIR> Backward compatibility with r2016b
                obj.mex_ = configuration.MEX;
            end
            
            if isfield(configuration,'MinGW') && isdir(configuration.MinGW) %#ok<ISDIR> Backward compatibility with r2016b
                obj.mingw_ = configuration.MinGW;
            end
            
            obj.isdirty_ = false;
            
        end
        
        function save(obj)
            
            folder = fileparts(obj.filename_);
            if ~isdir(folder) %#ok<ISDIR> Backward compatibility with r2016b
                mkdir(folder)
            end
            
            configuration = loadFile(obj);
            
            % Remove duplicates & empty platform (legacy)
            configuration(cellfun(@isempty,{configuration.Platform})) = [];
            [~,idx] = unique({configuration.Platform});
            configuration = configuration(idx);
            
            mask = strcmp({configuration.Platform},obj.platform_);
            
            if ~any(mask)
                mask = numel(configuration) + 1;
            end
            
            configuration(mask) = struct( ...
                'Platform',     obj.platform_,  ...
                'Adapter',      obj.adapter_,   ...
                'ED247',        obj.ed247_,     ...
                'LibXML2',      obj.libxml2_,   ...
                'MEX',          obj.mex_,       ...
                'MinGW',        obj.mingw_      ...
                );
            
            save(obj.filename_,'configuration')
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
        
        function configuration = loadFile(obj)
            
            configuration = {};
            if exist(obj.filename_,'file') == 2
                
                configuration = load(obj.filename_);
                
                if isfield(configuration,'configuration')
                    configuration = configuration.configuration;
                end
                
                if ~isfield(configuration,'Platform') % Field added after release
                    if ispc()
                        configuration.Platform = 'Windows';
                    else
                        configuration.Platform = 'Linux';
                    end
                end
                
                % Remove configurations with no Platform
                toremove = cellfun(@isempty,{configuration.Platform});
                configuration(toremove) = [];
                
            end
            
            if isempty(configuration)
                
                configuration = struct(...
                    'Platform',     obj.platform_, ...
                    'Adapter',      '', ...
                    'ED247',        '', ...
                    'LibXML2',      '', ...
                    'MEX',          '', ...
                    'MinGW',        ''  ...
                    );
            end
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = default(varargin)
            
            obj = ed247.Configuration.reset(varargin{:});
            reload(obj)
            
        end
        
        function obj = reset(varargin)
            
            defaultfilename = fullfile(ed247.Folder.DATA.Path,ed247.Configuration.FILE);
            
            if nargin >= 1 && ismember(varargin{1},{'Windows','Linux'})
                platform = varargin{1};
                varargin(1) = [];
            elseif ispc()
                platform = 'Windows';
            else
                platform = 'Linux';
            end
            
            defaultadapter  = fullfile(ed247.Folder.DEPENDENCIES.Path,'ed247_simulink_adapter');
            defaultlibxml2  = fullfile(ed247.Folder.DEPENDENCIES.Path,'libxml2');
            defaultmex      = ed247.Folder.LIBRARY.Path;
            
            if strcmp(platform,'Windows')
                defaulted247    = fullfile(ed247.Folder.DEPENDENCIES.Path,'ed247','windows','mingw4.9.2','x64');
                defaultmingw    = getenv('MW_MINGW64_LOC');
            else
                defaulted247    = fullfile(ed247.Folder.DEPENDENCIES.Path,'ed247','linux','gcc4.8.5','x64');
                defaultmingw    = '';
            end
            
            p = inputParser();
            p.addParameter('Filename',  defaultfilename,    @(x) ischar(x) && isdir(x)) %#ok<ISDIR> Backward compatibility with r2016b
            p.addParameter('Adapter',   defaultadapter,     @(x) ischar(x))
            p.addParameter('ED247',     defaulted247,       @(x) ischar(x))
            p.addParameter('LibXML2',   defaultlibxml2,     @(x) ischar(x))
            p.addParameter('MEX',       defaultmex,         @(x) ischar(x))
            p.addParameter('MinGW',     defaultmingw,       @(x) ischar(x))
            parse(p,varargin{:})
            
            if exist(p.Results.Filename, 'file') == 2
                obj = ed247.Configuration( p.Results.Filename );
                reload(obj)
            else
                obj = ed247.Configuration( p.Results.Filename, ...
                    'Adapter',      p.Results.Adapter,  ...
                    'ED247',        p.Results.ED247,    ...
                    'LibXML2',      p.Results.LibXML2,  ...
                    'MEX',          p.Results.MEX,      ...
                    'MinGW',        p.Results.MinGW);
            end
            
            obj.platform_ = platform;
            
        end
        
    end
    
end