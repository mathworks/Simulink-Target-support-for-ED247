classdef (Abstract) aBlock < matlab.mixin.SetGet
    %
    % Copyright 2021 The MathWorks, Inc.
    %
    
    %% ABSTRACT DEPENDENT PROPERTIES
    properties (Abstract, Dependent)
        DisplayText
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)        
        Configuration
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = protected)
        block_
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = aBlock(block,varargin)
            
            if ischar(block) || isstring(block)
                block = get_param(block,'Handle');
            end
            obj.block_ = block;
            
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
       
        function configuration = get.Configuration(obj)
           
            modelname   = bdroot(obj.block_);
            mdlwrksp    = get_param(modelname,'ModelWorkspace');
            
            if mdlwrksp.hasVariable('ED247Configuration')
                configuration = mdlwrksp.getVariable('ED247Configuration');
            else
                configuration = struct();
            end
            
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