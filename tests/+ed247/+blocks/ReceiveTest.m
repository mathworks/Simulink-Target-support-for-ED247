classdef (SharedTestFixtures={...
        tools.WorkFolderFixture({'ED247','ReceiveBlock'})}) ...
        ReceiveTest < matlab.unittest.TestCase    
    % RECEIVETEST
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
            
            modelname = 'receivesimplea429';
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
            set(receiveblock, 'enable_refresh', 'on')
            
            % [ EXERCISE ]
            % Run SIM to update diagram only (do not care about warnings)
            warning('off')
            sim(modelname,'StopTime','0');
            warning('on')
                        
            % [ VERIFY ]
            receivemask = ed247.blocks.Receive(receiveblock);
            actual = receivemask.PortLabel;
            Type = repmat({'output'},6,1);
            Number = (1:6)';
            Label = {'A429_BUS_1_MSG_1';'A429_BUS_1_MSG_1_refresh';'A429_BUS_1_MSG_2';'A429_BUS_1_MSG_2_refresh';'A429_BUS_1_MSG_3';'A429_BUS_1_MSG_3_refresh'};
            expected = table(Type,Number,Label);            
            testCase.verifyEqual(actual,expected, ...
                'Receive port labels does not match expected')
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end