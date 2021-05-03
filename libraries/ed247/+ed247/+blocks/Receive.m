classdef Receive < ed247.blocks.aBlock
    %
    % Copyright 2021 The MathWorks, Inc.
    %
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        PortLabel
        DisplayText
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Receive(block,varargin)
            obj@ed247.blocks.aBlock(block,varargin{:})
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