classdef Configuration < matlab.mixin.SetGet
    %
    % Copyright 2020 The MathWorks, Inc.
    %
    
    %% CONSTANT
    properties (Constant)
        FILE = '.metadata';
        MINGW_ENVIRONMENT_VARIABLE = 'MW_MINGW64_LOC';
        LIBXML2_FOLDER_VARIABLE = 'LIBXML_LOC';
        ED247_FOLDER_VARIABLE = 'ED247_LOC';
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
        
        Name
        Date
        Version
        Developer
        Maintainer
        
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
        
        name_
        date_
        version_
        developer_
        maintainer_
        
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
        
        function name = get.Name(obj)
            name = obj.name_;
        end
        
        function date = get.Date(obj)
            date = obj.date_;
        end
        
        function version = get.Version(obj)
            version = obj.version_;
        end
        
        function developer = get.Developer(obj)
            developer = obj.developer_;
        end
        
        function maintainer = get.Maintainer(obj)
            maintainer = obj.maintainer_;
        end
        
        function filename = get.Filename(obj)
            filename = obj.filename_;
        end
        
        function adapter = get.Adapter(obj) %#ok<MANU>
            adapter = ed247.Folder.ADAPTER.Path;
        end
        
        function ed247 = get.ED247(obj)
            
            envvar = getenv(obj.ED247_FOLDER_VARIABLE);
            if isdir(envvar) %#ok<ISDIR> Backward compatibility with r2016b
                ed247 = envvar;
            elseif isdir(obj.ed247_) %#ok<ISDIR> Backward compatibility with r2016b
                ed247 = obj.ed247_;
            else
                ed247 = '';
            end
            
        end
        
        function libxml2 = get.LibXML2(obj)
            
            envvar = getenv(obj.LIBXML2_FOLDER_VARIABLE);
            if isdir(envvar) %#ok<ISDIR> Backward compatibility with r2016b
                libxml2 = envvar;
            elseif isdir(obj.libxml2_) %#ok<ISDIR> Backward compatibility with r2016b
                libxml2 = obj.libxml2_;
            else
                libxml2 = '';
            end
            
        end
        
        function mex = get.MEX(obj) %#ok<MANU>
            mex = ed247.Folder.LIBRARY.Path;            
        end
        
        function mingw = get.MinGW(obj)
            
            envvar = getenv(obj.MINGW_ENVIRONMENT_VARIABLE);
            if isdir(envvar) %#ok<ISDIR> Backward compatibility with r2016b
                mingw = envvar;
            elseif isdir(obj.mingw_) %#ok<ISDIR> Backward compatibility with r2016b
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
        
        function set.ED247(obj,ed247)
            obj.ed247_ = ed247;
            obj.isdirty_ = true;
        end
        
        function set.LibXML2(obj,libxml2)
            obj.libxml2_ = libxml2;
            obj.isdirty_ = true;
        end
                
        function set.MinGW(obj,mingw)
            obj.mingw_ = mingw;
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
                
                mingwBinPath = fullfile(obj.MinGW,'bin');
                if isdir(mingwBinPath) %#ok<ISDIR> Backward compatibility with r2016b
                    
                    fprintf(1, '## Remove existing MinGW64 installation folder from Windows path\n');
                    p = p(cellfun(@(x) isempty(regexpi(x,'mingw')),p));
                    curPath = strjoin(p,';');
                    
                    fprintf(1, '## Add MinGW64 installation folder (%s) in Windows path\n', mingwBinPath);
                    curPath = strjoin({mingwBinPath,curPath},';');
                    
                    fprintf(1, '## Configure MEX to use MinGW64 as compiler\n');
                    
                    fprintf(1, '\t- Set "%s" environment variable to "%s"\n', obj.MINGW_ENVIRONMENT_VARIABLE, obj.MinGW);
                    setenv(obj.MINGW_ENVIRONMENT_VARIABLE, obj.MinGW)
                    
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
                            warning('ED247:invalidPath', 'MinGW64 installation folder "%s" is not a valid MinGW64 installation folder.\nTo be valid, the installation folder should contains a bin subfolder', obj.MinGW)
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
            
            [configuration,groups] = loadFile(obj);
            
            common = configuration{strcmpi(groups,'common')};
            
            if isfield(common,'Name')
                obj.name_ = common.Name;
            end
            
            if isfield(common,'Date')
                obj.date_ = common.Date;
            end
            
            if isfield(common,'Version')
                obj.version_ = common.Version;
            end
            
            if isfield(common,'Developer')
                obj.developer_ = common.Developer;
            end
            
            if isfield(common,'Maintainer')
                obj.maintainer_ = common.Maintainer;
            end
            
            if any(strcmp(groups,obj.platform_))
                configuration = configuration(strcmp(groups,obj.platform_));
            end
            
            if numel(configuration) > 0
                configuration = configuration{1};
            elseif isempty(configuration)
                configuration = struct();
            end
                        
            if isfield(configuration,'ED247') && isdir(configuration.ED247) %#ok<ISDIR> Backward compatibility with r2016b
                obj.ed247_ = configuration.ED247;
            end
            
            if isfield(configuration,'LibXML2') && isdir(configuration.LibXML2) %#ok<ISDIR> Backward compatibility with r2016b
                obj.libxml2_ = configuration.LibXML2;
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
            
            [configuration,groupnames] = loadFile(obj);
            
            % Remove duplicates & empty platform (legacy)
            configuration(cellfun(@isempty,groupnames)) = [];
            [~,idx] = unique(groupnames,'stable');
            configuration = configuration(idx);
                        
            %
            % Platform dependent
            %
            mask = strcmp(groupnames,obj.platform_);
            
            if ~any(mask)
                mask = numel(configuration) + 1;
                groupnames{end+1} = obj.platform_;
            end
            
            configuration{mask} = struct( ...
                'ED247',        obj.ed247_,     ...
                'LibXML2',      obj.libxml2_,   ...
                'MinGW',        obj.mingw_      ...
                );
            
            saveFile(obj,configuration,groupnames)
            
        end
        
        function reset(obj)
                        
            %
            % Common
            %
            configuration = struct( ...
                'Name',         obj.name_,      ...
                'Date',         obj.date_,      ...
                'Version',      obj.version_,   ...
                'Developer',    obj.developer_, ...
                'Maintainer',   obj.maintainer_ ...
                );
            
            saveFile(obj,{configuration},{'Common'})
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
        
        function [configuration,groupnames] = loadFile(obj)
                                        
            if exist(obj.filename_,'file') == 2
                
                configtxt = fileread(obj.filename_);                
                configtxt = deblank(strsplit(configtxt,'\n')');
                
                groups = startsWith(configtxt, '---');
                groups = cumsum(groups);
                
                groupnames = cell(length(unique(groups)),1);
                configuration = cell(length(unique(groups)),1);
                for i_group = unique(groups)'
                    
                    group = configtxt(groups == i_group);
                    
                    name = regexp(group{1}, '--- (.*?) ---', 'tokens');                    
                    name = char(name{1});
                    groupnames{i_group} = name;
                    
                    props = textscan(strjoin(group(2:end),'\n'), '%s %s', 'Delimiter', '=');
                    if length(props{2}) == length(props{1}) - 1
                        props{2}(end+1) = {''};
                    end
                    props = [props{:}];                                        
                    configuration{i_group} = cell2struct(props(:,2),props(:,1),1);
                    
                end
            
            else
                groupnames = {};
                configuration = {};
            end
                        
        end
        
        function saveFile(obj,configuration,groupnames)
           
            fid = fopen(obj.filename_,'wt');
            if fid > 0
                closeFile = onCleanup(@() fclose(fid));
                
                for i_group = 1:length(groupnames)
                    
                    fprintf(fid, '--- %s ---\n', groupnames{i_group});
                    
                    props = configuration{i_group};
                    props = [fieldnames(props),struct2cell(props)]';
                    fprintf(fid, '%s=%s\n', props{:});
                    
                end
                
            end
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = default(varargin)
            
            defaultfilename = fullfile(ed247.Folder.ROOT.Path,ed247.Configuration.FILE);
            
            if nargin >= 1 && ismember(varargin{1},{'Windows','Linux'})
                platform = varargin{1};
                varargin(1) = [];
            elseif ispc()
                platform = 'Windows';
            else
                platform = 'Linux';
            end
            
            defaultlibxml2  = '';
            
            if strcmp(platform,'Windows')
                defaulted247    = '';
                defaultmingw    = getenv('MW_MINGW64_LOC');
            else
                defaulted247    = '';
                defaultmingw    = '';
            end
            
            p = inputParser();
            p.addParameter('Filename',  defaultfilename,    @(x) ischar(x) && isdir(x)) %#ok<ISDIR> Backward compatibility with r2016b
            p.addParameter('ED247',     defaulted247,       @(x) ischar(x))
            p.addParameter('LibXML2',   defaultlibxml2,     @(x) ischar(x))
            p.addParameter('MinGW',     defaultmingw,       @(x) ischar(x))
            parse(p,varargin{:})
            
            if exist(p.Results.Filename, 'file') == 2
                obj = ed247.Configuration( p.Results.Filename );
                reload(obj)
            else
                obj = ed247.Configuration( p.Results.Filename, ...
                    'ED247',        p.Results.ED247,    ...
                    'LibXML2',      p.Results.LibXML2,  ...
                    'MinGW',        p.Results.MinGW);
            end
            
            obj.platform_ = platform;
            
            reload(obj)
            
        end
        
    end
    
end