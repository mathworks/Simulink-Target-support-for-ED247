classdef Receive < ed247.blocks.aBlock
    %
    % Copyright 2021 The MathWorks, Inc.
    %
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        DisplayText
        IsRefresh
        PortLabel
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Receive(block,varargin)
            obj@ed247.blocks.aBlock(block,varargin{:})
        end
        
    end
    
    %% ACCESSORS
    methods
                
        function displaytext = get.DisplayText(obj) %#ok<MANU>
            displaytext = 'ED247 Receive';
        end
        
        function isrefresh = get.IsRefresh(obj) %#ok<MANU>
            isrefresh = true; % TODO
        end
        
        function portlabel = get.PortLabel(obj) %#ok<MANU>
            Type    = {}; % 'output'
            Number  = [];
            Label   = {};
            portlabel = table(Type, Number, Label);
        end
                
    end
    
    %% PUBLIC METHODS
    methods
        
        function initialize(obj)
                    
            m = Simulink.Mask.get(obj.block_);
            refreshgroup = m.getDialogControl('refresh_group');
            
            if obj.IsRefresh
                refreshgroup.Visible = 'on';
                set(obj.block_,'refresh_factor','1')
            else
                refreshgroup.Visible = 'off';
                set(obj.block_,'refresh_factor','0')
            end
            
        end
        
        function enableRefresh(obj)
                        
            m = Simulink.Mask.get(obj.block_);
            refreshfactor = m.Parameters(strcmp({m.Parameters.Name},'refresh_factor'));
            
            userdata = get(obj.block_,'UserData');
            
            enablerefresh = get(obj.block_,'enable_refresh');
            if strcmp(enablerefresh,'on')
                refreshfactor.Enabled = 'on';
                if isfield(userdata,'RefreshFactor') && userdata.RefreshFactor > 0
                    set(obj.block_,'refresh_factor',num2str(userdata.RefreshFactor))
                else
                    set(obj.block_,'refresh_factor','1')
                end
            else
                refreshfactor.Enabled = 'off';
                userdata.RefreshFactor = str2double(get(obj.block_,'refresh_factor'));
                set(obj.block_,'UserData',userdata)
                set(obj.block_,'refresh_factor','0')
            end
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Receive(gcbh);
        end
        
    end
    
end