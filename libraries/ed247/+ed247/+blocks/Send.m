classdef Send < ed247.blocks.aBlock
    %
    % Copyright 2021 The MathWorks, Inc.
    %
    
    %% CONSTANT
    properties (Constant)
        TYPES_FOR_REFRESH = {'A429','A664','A825'};
    end
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        DisplayText
        IsRefresh
        PortLabel
    end
    
    %% CONSTRUCTOR
    methods
        
        function obj = Send(block,varargin)
            obj@ed247.blocks.aBlock(block,varargin{:})
            obj.assert(bdIsLibrary(bdroot(obj.block_)) || strcmp(get(obj.block_,'ReferenceBlock'),'lib_ed247/ED247_Send'), ...
                'Block shall be a ED247 Send block')
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function displaytext = get.DisplayText(obj)
            
            if bdIsLibrary(bdroot(obj.block_)) || strcmp(get(obj.block_,'show_port_labels'),'off')
                displaytext = 'ED247 Send';
            else
                displaytext = '';
            end
            
        end
        
        function isrefresh = get.IsRefresh(obj)
            configuration = obj.Configuration;
            if ~isempty(configuration)
                isrefresh = any(ismember({configuration.signal_type},obj.TYPES_FOR_REFRESH));
            else
                isrefresh = true;
            end
        end
        
        function portlabel = get.PortLabel(obj)
                        
            if strcmp(get(obj.block_,'show_port_labels'),'on')                
                portlabel = getPorts(obj);
            else
                inports = get(obj.block_,'PortHandles');
                inports = inports.Inport;
                nports = numel(inports);
                portlabel = table(repmat({'input'},nports,1), (1:nports)', repmat({''},nports,1), ...
                    'VariableNames', {'Type','Number','Label'});
            end
            
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function initialize(obj) %#ok<MANU> Called by block callback
            
        end
                
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
       
        function ports = getPorts(obj)
            %
            % Create a table with port information
            %   - Name
            %   - Type (input/output)
            %   - Number = port number
            %
            % If refresh is disabled the port list is the list of block
            % input signals (signals direction =OUT)
            % When refresh is enabled, some signals will be associated with
            % a refresh port (will be the following port)
            %
            
            configuration  = obj.Configuration;
            isrefresh = obj.IsRefresh && strcmp(get(obj.block_,'enable_refresh'),'on');
                        
            if ~isempty(configuration)
                
                inputsignals   = configuration(ismember({configuration.direction},{'OUT','INOUT'}));
                
                names = {inputsignals.name};
                
                if isrefresh
                    % If refresh is enabled, add signal name with
                    % "_refresh" suffix as 2nd row
                    names(2,:) = strcat({inputsignals.name},'_refresh');
                    names(2,~ismember({inputsignals.signal_type},obj.TYPES_FOR_REFRESH)) = {''};
                end
                
                % Remove empty signal names from array -> will be the
                % non-refreshed signals and make sure that names vector is
                % a column-vector
                names = names(~cellfun(@isempty,names));
                names = names(:);
                
                ports = table(repmat({'input'},size(names)), (1:length(names))', names, ...
                    'VariableNames', {'Type','Number','Label'});
                
            else
                ports = table();
            end
            
        end
        
    end
    
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Send(gcbh);
        end
        
    end
    
end