classdef (SharedTestFixtures={...
        tools.WorkFolderFixture({'ED247','Issue018'})}) ...
        Issue018 < matlab.unittest.TestCase
    % ISSUE018
    %
    % Failed to read XML configuration files (ECIC/CMD)
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
        
        function testReadSimpleA429WithRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'readsimplea429';
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
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(receiveblock, 'enable_refresh', 'on')
            set(sendblock, 'enable_refresh', 'on')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            % [ VERIFY ]
            receiveports = get(receiveblock,'PortHandles');
            testCase.verifyLength(receiveports.Outport,6, ...
                sprintf('[A429] Receive block should have %d ports (%d x2 as refresh is enabled)', 6, 3))
            
            sendports = get(sendblock,'PortHandles');
            testCase.verifyLength(sendports.Inport,4, ...
                sprintf('[A429] Send block should have %d ports (%d x2 as refresh is enabled)', 4, 2))
            
        end
        
        function testReadSimpleA429WithoutRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'readsimplea429';
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
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(receiveblock, 'enable_refresh', 'off')
            set(sendblock, 'enable_refresh', 'off')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            % [ VERIFY ]
            receiveports = get(receiveblock,'PortHandles');
            testCase.verifyLength(receiveports.Outport,3, ...
                sprintf('[A429] Receive block should have %d ports (refresh is disabled)', 3))
            
            sendports = get(sendblock,'PortHandles');
            testCase.verifyLength(sendports.Inport,2, ...
                sprintf('[A429] Send block should have %d ports (refresh is disabled)', 2))
            
        end
        
        function testReadELACSimplifiedWithRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_simplified.zip');
            unzip(archivename,pwd)
            
            modelname = 'readsimpleelac';
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
            set(configurationblock, 'configurationFilename', '''ELACe2M_ECIC.xml''')
            set(receiveblock, 'enable_refresh', 'on')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            % [ VERIFY ]
            receiveports = get(receiveblock,'PortHandles');
            testCase.verifyLength(receiveports.Outport,136, ...
                sprintf('[A429] Receive block should have %d ports (%d x2 as refresh is enabled)', 136, 68))
            
        end
        
        function testReadELACSimplifiedWithoutRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_simplified.zip');
            unzip(archivename,pwd)
            
            modelname = 'readsimpleelac';
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
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2M_ECIC.xml''')
            set(receiveblock, 'enable_refresh', 'off')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            % [ VERIFY ]
            receiveports = get(receiveblock,'PortHandles');
            testCase.verifyLength(receiveports.Outport,68, ...
                sprintf('[A429] Receive block should have %d ports (refresh is disabled)', 68))
            
        end
        
        function testReadELACWithRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readelac';
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
            set(configurationblock, 'configurationFilename', '''ELACe2M_ECIC.xml''')
            set(receiveblock, 'enable_refresh', 'on')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            % [ VERIFY ]
            receiveports = get(receiveblock,'PortHandles');
            testCase.verifyLength(receiveports.Outport,968, ...
                sprintf('[A429] Receive block should have %d ports (%d x2 as refresh is enabled)', 1936, 968))
            
        end
        
        function testReadELACWithoutRefresh(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readelac';
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
            %   - Disable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2M_ECIC.xml''')
            set(receiveblock, 'enable_refresh', 'off')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
            
            % [ VERIFY ]
            receiveports = get(receiveblock,'PortHandles');
            testCase.verifyLength(receiveports.Outport,968, ...
                sprintf('[A429] Receive block should have %d ports (refresh is disabled)', 968))
            
        end
                
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end