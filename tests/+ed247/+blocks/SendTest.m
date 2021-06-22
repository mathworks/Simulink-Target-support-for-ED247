classdef (SharedTestFixtures={...
        tools.WorkFolderFixture({'ED247','SendBlock'})}) ...
        SendTest < matlab.unittest.TestCase    
    % SENDTEST
    %
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
       
        function testDisplayIssue018(testCase)
            %
            % Derivated from Issue018 test
            %
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'sendsimplea429';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
             %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(sendblock, 'enable_refresh', 'on', 'show_port_labels', 'on')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
                        
            % [ VERIFY ]
            sendports = get(sendblock,'PortHandles');
            testCase.verifyLength(sendports.Inport,4, ...
                sprintf('Send block should have %d ports (refresh is disabled)', 4))
            
            sendmask = ed247.blocks.Send(sendblock);
            actual = sendmask.PortLabel;
            Type = repmat({'input'},4,1);
            Number = (1:4)';
            Label = {'A429_BUS_2_MSG_1';'A429_BUS_2_MSG_1_refresh';'A429_BUS_2_MSG_2';'A429_BUS_2_MSG_2_refresh'};
            expected = table(Type,Number,Label);            
            testCase.verifyEqual(actual,expected, ...
                'Send port labels does not match expected')
            
        end
        
        function testDisplayWithoutRefresh(testCase)
            %
            % Derivated from Issue018 test
            %
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'sendsimplea429';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
             %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''a429_01_ecic_in.xml''')
            set(sendblock, 'enable_refresh', 'off', 'show_port_labels', 'on')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
                        
            % [ VERIFY ]
            sendports = get(sendblock,'PortHandles');
            testCase.verifyLength(sendports.Inport,2, ...
                sprintf('Send block should have %d ports (refresh is disabled)', 2))
            
            sendmask = ed247.blocks.Send(sendblock);
            actual = sendmask.PortLabel;
            Type = repmat({'input'},2,1);
            Number = (1:2)';
            Label = {'A429_BUS_2_MSG_1';'A429_BUS_2_MSG_2'};
            expected = table(Type,Number,Label);            
            testCase.verifyEqual(actual,expected, ...
                'Send port labels does not match expected')
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end