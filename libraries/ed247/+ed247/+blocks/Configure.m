classdef Configure < ed247.blocks.aBlock
    %
    % Copyright 2020 The MathWorks, Inc.
    %
    
    %% DEPENDENT PROPERTIES
    properties (Dependent)
        ConfigurationFile
        ConfigurationFileInt8
        DisplayText
        LogFileInt8
    end
            
    %% CONSTRUCTOR
    methods
        
        function obj = Configure(block,varargin)
            obj@ed247.blocks.aBlock(block,varargin{:})
        end
        
    end
    
    %% ACCESSORS
    methods
        
        function configurationfile = get.ConfigurationFile(obj)
            
            filename = get_param(obj.block_,'configurationFilename');
            if isempty(filename)
                configurationfile = '';
                return
            end
            
            configurationfile = regexp(filename,'^''(.*?)''$','tokens');            
            if isempty(configurationfile) && ~isempty(filename)                
                configurationfile = evalin('base',filename);                
            else
                configurationfile = char(configurationfile{1});
            end
            
            if ~isempty(which(configurationfile))
                configurationfile = which(configurationfile);
            elseif exist(configurationfile,'file') ~= 2
                obj.error('ED247:IOVariableNumber:invalidFile', ...
                    'Cannot find file %s', configurationfile)
            end
            
        end
        
        function configurationfileint8 = get.ConfigurationFileInt8(obj)            
            
            configurationfile = obj.ConfigurationFile;
            if isempty(configurationfile)
                configurationfileint8 = int8(0);
            else
                configurationfileint8 = int8([configurationfile,0]);
            end
            
        end
        
        function displaytext = get.DisplayText(obj)
            
            filename = get_param(obj.block_,'configurationFilename');
            filename = regexprep(filename,'^''|''$','');
            
            if isempty(filename)
                displaytext = 'ED247';
            else
                [~,file,ext] = fileparts(filename);
                displaytext = [file,ext];
            end
            
        end
        
        function logfileint8 = get.LogFileInt8(obj)
            
            isenable = get_param(obj.block_,'enableLog');
            filename = get_param(obj.block_,'logFilename');
            if isempty(filename) || strcmp(isenable,'off')
                logfileint8 = int8(0);
                return
            end
            
           logfileint8 = regexp(filename,'^''(.*?)''$','tokens'); 
           logfileint8 = char(logfileint8{1});
           
           proj = slproject.getCurrentProjects();
            if numel(proj) == 1 && startsWith(logfileint8,'<PROJECT_FOLDER>') % A Simulink project is opened and selected path is relative to project folder
                logfileint8 = strrep(logfileint8,'<PROJECT_FOLDER>',proj.RootFolder);
            elseif startsWith(logfileint8,'<PROJECT_FOLDER>') % No project open -> use current folder
                logfileint8 = strrep(logfileint8,'<CURRENT_FOLDER>',pwd);
            elseif startsWith(logfileint8,'<CURRENT_FOLDER>')
                logfileint8 = strrep(logfileint8,'<CURRENT_FOLDER>',pwd);
            end
           
           logfileint8 = int8([logfileint8,0]); 
           
        end
        
    end
    
    %% PUBLIC METHODS
    methods
       
        function update(obj)
            
            enableLog = get_param(obj.block_,'enableLog');
            
            m = Simulink.Mask.get(obj.block_);
            if ~isempty(m.Parameters)
                logFilename = m.Parameters(strcmp({m.Parameters.Name},'logFilename'));
                browseLogFile   = m.getDialogControl('browseLogFile');
                                
                logFilename.Enabled = enableLog;
                browseLogFile.setEnabled(enableLog)
            end
            
        end
        
        function save(obj)
            % Save data to Model workspace
            obj.print("No action (deprecated)")
        end
        
    end
    
    %% BLOCK CALLBACKS
    methods
        
        function LoadFcn(obj)
            
            try
                
                islibrary = bdIsLibrary(bdroot(obj.block_));
                if ~islibrary
                    saveConfigurationInModelWorkspace(obj)
                end
                
            catch me
                if ~strcmp(me.identifier,'ED247:IOVariableNumber:invalidFile')
                    rethrow(me)
                end
            end
            
        end
       
        function InitFcn(obj)
            
            config = ed247.Configuration.default();
            config.addLibrariesToPath()
                   
            update(obj)
            
        end
        
    end
    
    %% PUBLIC METHODS
    methods
        
        function browseConfigurationFile(obj)
            
            filename = get_param(obj.block_,'configurationFilename');
            if contains(filename,'''')
                filename = strrep(filename,'''','');
            elseif contains(filename,'"')
                filename = strrep(filename,'"','');
            end
            [filename,pathname] = uigetfile({'*.xml','ECIC configuration file'}, 'Select configuration file', filename);
            if pathname == 0; return; end
            
            mlpath = which(filename);
            if isempty(mlpath)
                filename = fullfile(pathname,filename);
            end
            set_param(obj.block_, 'configurationFilename', sprintf('''%s''', filename))
            
            saveConfigurationInModelWorkspace(obj)
            
        end
        
        function browseLogFile(obj)
            
            filename = get_param(obj.block_,'logFilename');
            if contains(filename,'''')
                filename = strrep(filename,'''','');
            elseif contains(filename,'"')
                filename = strrep(filename,'"','');
            end
            [filename,pathname] = uiputfile({'*.log','LOG file';'*.txt','TXT file';'*.*','All files'}, 'Define log file', filename);
            if pathname == 0; return; end
                      
            proj = slproject.getCurrentProjects();
            if numel(proj) == 1 && startsWith(pathname,proj.RootFolder) % A Simulink project is opened and selected path is relative to project folder
                pathname = strrep(pathname,proj.RootFolder,'<PROJECT_FOLDER>');
            elseif startsWith(pathname,pwd)
                pathname = strrep(pathname,pwd,'<CURRENT_FOLDER>');
            end
            
            filename = fullfile(pathname,filename);
            set_param(obj.block_, 'logFilename', sprintf('''%s''', filename))
            
        end
        
        function enableLogFile(obj)
            update(obj)
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
       
        function saveConfigurationInModelWorkspace(obj)
            
            modelname   = bdroot(obj.block_);
            mdlwrksp    = get_param(modelname,'ModelWorkspace');
            
            filename = obj.ConfigurationFile;
            if ~isempty(filename)
                ecic = ed247.ECIC(filename);
                read(ecic)
                assignin(mdlwrksp, 'ED247Configuration', ecic.Configuration)
            end
            
        end
        
    end
        
    %% STATIC METHODS
    methods (Static)
        
        function obj = fromCurrent(varargin)
            obj = ed247.blocks.Configure(gcbh);
        end
        
        function obj = fromModel(varargin)
            
            if ~isempty(varargin)
                modelname = varargin{1};
            else
                modelname = bdroot;
            end
            
            block = find_system(modelname,"FindAll",'on',"ReferenceBlock","lib_ed247/ED247_Configuration");
            assert(numel(block) == 1, "Model '%s' should contain 1 ED247 Configure block (currently %d)", ...
                modelname, numel(block))
            
            obj = ed247.blocks.Configure(block);
            
        end
        
    end
    
end