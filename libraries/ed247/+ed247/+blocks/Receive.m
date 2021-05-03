classdef Receive < matlab.mixin.SetGet
   
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        PortLabel
        DisplayText
    end
    
    %% IMMUTABLE PROPERTIES
    properties (SetAccess = immutable, GetAccess = private)
        block_
    end
    
    %% CONSTRUCTOR
    methods
       
        function obj = Receive(block,varargin)
           
            obj.block_ = block;
            if ~isempty(varargin)
                set(obj,varargin{:})
            end
            
        end
        
    end
    
    %% ACCESSORS
    methods
       
        function portlabel = get.PortLabel(obj) %#ok<MANU>
            Type    = {}; % 'output'
            Number  = [];
            Label   = {};
            portlabel = table(Type, Number, Label);
        end
        
        function displaytext = get.DisplayText(obj) %#ok<MANU>
            displaytext = 'ED247 Receive';
        end
        
    end
    
    %% PUBLIC METHODS
    methods
       
        function initialize(obj)
            
            obj
            
        end
                
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Receive(gcbh);
        end
        
    end
    
end