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
        ECICFile
        ICDFiles
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
           
            configuration = struct.empty;
            
            modelname   = bdroot(obj.block_);
            mdlwrksp    = get_param(modelname,'ModelWorkspace');
            
            if ~isempty(mdlwrksp) && mdlwrksp.hasVariable('ED247Configuration')
                
                ecic = ed247.ECIC.fromStruct(mdlwrksp.getVariable('ED247Configuration'));
                if ~isempty(ecic)
                    configuration = ecic.Configuration;
                end
                
            end
            
        end
        
        function ecicfile = get.ECICFile(obj)
           
            modelname   = bdroot(obj.block_);
            mdlwrksp    = get_param(modelname,'ModelWorkspace');
            
            if ~isempty(mdlwrksp) && mdlwrksp.hasVariable('ED247Configuration') && numel(mdlwrksp.getVariable('ED247Configuration')) == 1
                ecic = ed247.ECIC.fromStruct(mdlwrksp.getVariable('ED247Configuration'));
                ecicfile = ecic.ECICFile;
            else
                ecicfile = '';
            end
            
        end
        
        function icdfiles = get.ICDFiles(obj)
           
            modelname   = bdroot(obj.block_);
            mdlwrksp    = get_param(modelname,'ModelWorkspace');
            
            if ~isempty(mdlwrksp) && mdlwrksp.hasVariable('ED247Configuration')
                ecic = ed247.ECIC.fromStruct(mdlwrksp.getVariable('ED247Configuration'));
                icdfiles = ecic.ICDFiles;
            else
                icdfiles = {};
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