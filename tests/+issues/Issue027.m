classdef (SharedTestFixtures={...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_mock')), ...
        tools.WorkFolderFixture({'ED247','Issue027'})}) ...
        Issue027 < matlab.unittest.TestCase
    % ISSUE027
    %
    % Lines are disconnected during update
    %
    % Block Receive has its outputs disconnected sometimes after model update.
    %
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function setFileFolder(testCase)
            proj = currentProject();
            testCase.filefolder_ = fullfile(proj.RootFolder,'tests','_files');
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testReadSimpleA429(testCase)
                        
            % [ SETUP ]
            warning('off')
            resetWarnings = onCleanup(@() warning('on'));
            
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'simplea429';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            receiveblockname = [modelname,'/Receive'];
            receiveblock = add_block('lib_ed247/ED247_Receive', receiveblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(receiveblock, 'enable_refresh', 'on', 'show_port_labels', 'on')
            
            sim(modelname,'StopTime','0');
            
            recv = ed247.blocks.Receive(receiveblock);
            recv.fWaitbar = @(varargin) MockWaitbar.waitbar(varargin{:});
            createBus(recv)
            
            ports = get(receiveblock,'PortHandles');
            testCase.assumeGreaterThan(ports.Outport,-1, ...
                'All block outports should be connected')
            
            % [ EXERCISE ]
            sim(modelname,'StopTime','0');
            set_param(modelname,'SolverType','Fixed-Step','SystemTargetFile','slrealtime.tlc','GenCodeOnly','on','LaunchReport','off')
            slbuild(modelname)
            
            % [ VERIFY ]
            testCase.verifyGreaterThan(ports.Outport,-1, ...
                'All block outports should be connected')
            
        end
        
        function testReadSimpleA429WithRefresh(testCase)
                        
            % [ SETUP ]
            warning('off')
            resetWarnings = onCleanup(@() warning('on'));
            
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'fullelac';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            receiveblockname = [modelname,'/Receive'];
            receiveblock = add_block('lib_ed247/ED247_Receive', receiveblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            set(receiveblock, 'enable_refresh', 'on', 'show_port_labels', 'on')
            
            sim(modelname,'StopTime','0');
            
            recv = ed247.blocks.Receive(receiveblock);
            recv.fWaitbar = @(varargin) MockWaitbar.waitbar(varargin{:});
            createBus(recv)
            
            ports = get(receiveblock,'PortHandles');
            testCase.assumeGreaterThan(ports.Outport,-1, ...
                'All block outports should be connected')
            
            % [ EXERCISE ]
            sim(modelname,'StopTime','0');
            set_param(modelname,'SolverType','Fixed-Step','SystemTargetFile','slrealtime.tlc','GenCodeOnly','on','LaunchReport','off')
            slbuild(modelname)
            
            % [ VERIFY ]
            testCase.verifyGreaterThan(ports.Outport,-1, ...
                'All block outports should be connected')
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end