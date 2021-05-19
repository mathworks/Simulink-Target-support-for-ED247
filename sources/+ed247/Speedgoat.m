classdef Speedgoat < matlab.mixin.SetGet
   
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        Executable
        ModelName
    end
    
    %% PUBLIC PROPERTIES
    properties
        Target          (1,1)   string = "slrealtime";
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        buildfolder_    (1,1)   string
        executable_     (1,1)   string
        modelname_      (1,1)   string
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = Speedgoat(modelname,varargin)
            
            obj.assert(~verLessThan("MATLAB","9.9"),"ED247 support export to Speedgoat for MATLAB releases greater than r2020b (included)")
            
            obj.modelname_ = modelname;
            
            codegenfolder       = Simulink.fileGenControl("get","CodeGenFolder");
            obj.executable_     = fullfile(codegenfolder, obj.modelname_ + ".mldatx");
            obj.buildfolder_    = fullfile(codegenfolder, obj.modelname_ + "_" + obj.Target + "_rtw");
                                    
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
       
        function executable = get.Executable(obj)
            executable = obj.executable_;
        end
        
        function modelname = get.ModelName(obj)
            modelname = obj.modelname_;
        end
        
    end
    
    %% PUBLIC METHODS
    methods
       
        function configure(obj)
            
            config = getActiveConfigSet(obj.modelname_);
            if isa(config,"Simulink.ConfigSetRef")
                
                configname = config.SourceName;
                dict = get_param(obj.modelname_,"DataDictionary");
                if ~isempty(dict)
                    d = Simulink.data.dictionary.open(dict);
                    wrkspace = d.getSection("Configurations");
                else
                    wrkspace = "base";
                end
                config = evalin(wrkspace,configname);
                
            end
            
            % Set Mandatory parameters
            obj.print("Apply mandatory parameters:")
            obj.print("\t-SolverType = 'Fixed-Step'")
            obj.print("\t-SystemTargetFile = '%s.tlc'", obj.Target)
            obj.print("\t-GenCodeOnly = 'off'")
            
            set_param(config, ...
                "SolverType",       "Fixed-Step", ...
                "SystemTargetFile", obj.Target + ".tlc", ...
                "GenCodeOnly",      "off")
            
        end
        
        function compile(obj)
                     
            obj.print("Build model with SLBUILD")
            slbuild(obj.modelname_)
                                    
        end
        
        function package(obj)
           
            c = ed247.blocks.Configure.fromModel(obj.modelname_);
            [~,f] = ed247_mex(c.ConfigurationFile);
            configurationfiles = unique([{c.ConfigurationFile};f]);
            configurationfiles(cellfun(@isempty,configurationfiles)) = [];
            configurationfiles = convertCharsToStrings(configurationfiles);
            
            [~,configurationnames] = fileparts(configurationfiles);
            configurationnames = configurationnames + ".xml";
            
            obj.print("Add configuration file (%s) to executable", strjoin(configurationnames,","))
            appObj = slrealtime.internal.Application(obj.executable_);
            arrayfun(@(n,f) appObj.add("/bin/" + n,f), configurationnames, configurationfiles)
            
        end
        
        function cleanup(obj)
           
            if isfolder(obj.buildfolder_)
                obj.print("Delete build folder : '%s'", obj.buildfolder_)
                rmdir(obj.buildfolder_, "s")
            else
                obj.print("No build folder")
            end
            
            if exist(obj.executable_,"file") == 2
                obj.print("Delete executable : '%s'", obj.executable_)
                delete(obj.executable_)
            else
                obj.print("No executable found")
            end
                                                
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
       
        function assert(obj,varargin) %#ok<INUSL>
            assert(varargin{:})
        end
        
        function error(obj,varargin) %#ok<INUSL>
            error(varargin{:})
        end
        
        function warning(obj,varargin) %#ok<INUSL>
            warning(varargin{:})
        end
        
        function print(obj,varargin) %#ok<INUSL>
            fprintf(1,varargin{1} + "\n", varargin{2:end})
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
       
        function varargout = deployModel(modelname,varargin)
            
            ismodelloaded = bdIsLoaded(modelname);
            if ~ismodelloaded
                load_system(modelname)
            end
            
            obj = ed247.Speedgoat(modelname,varargin{:});
            configure(obj)
            compile(obj)
            package(obj)
            
            if nargout
                varargout = {obj};
            elseif ~ismodelloaded
                bdclose(modelname)
            end
            
        end
        
    end
        
end