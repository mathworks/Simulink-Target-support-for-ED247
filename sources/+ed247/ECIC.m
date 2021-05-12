classdef ECIC < matlab.mixin.SetGet
   
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        Configuration
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        ECICFile
        ICDFiles
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        configuration_
        icdfiles_
    end
        
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        filename_
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = ECIC(filename,varargin)
            obj.filename_ = filename;
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
        end
        
    end
    
    %% ACCESSORS
    methods
       
        function configuration = get.Configuration(obj)
            configuration = obj.configuration_;
        end
        
        function ecicfile = get.ECICFile(obj)
            ecicfile = obj.filename_;
        end
        
        function icdfiles = get.ICDFiles(obj)
            icdfiles = obj.icdfiles_;
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function read(obj)
            
            obj.assert(exist(obj.filename_,'file') == 2, 'File "%s" does not exist', obj.filename_)
            
            if ~isempty(which(obj.filename_))
                filename = which(obj.filename_);
            else
                filename = char(obj.filename_);
            end
            [configuration,icdfiles] = ed247_mex(filename);
            
            obj.configuration_ = configuration;
            obj.icdfiles_ = unique(icdfiles);
            
        end
        
    end
    
    %% PROTECTED HELPERS
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
            fprintf(1,varargin{:})
        end
        
    end
    
end