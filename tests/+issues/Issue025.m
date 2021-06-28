classdef (SharedTestFixtures={...
        tools.WorkFolderFixture({'ED247','Issue025'})}) ...
        Issue025 < matlab.unittest.TestCase
    % ISSUE025
    %
    % No more than 128 messages per stream
    %
    % Streams which contains more than 128 messages are not fully loaded (ex. DGO03 - outputs)
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
                
        function testReadELACFullConfigureOnly(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'ELAC_full.zip');
            unzip(archivename,pwd)
            
            modelname = 'readfullelac';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            
            % [ EXERCISE ]                        
            sim(modelname,'StopTime','0');            
            
            % [ VERIFY ]
            mdlWrks = get_param(modelname,'ModelWorkspace');
            config = mdlWrks.evalin('ED247Configuration');
            
            actual = nnz(strcmp({config.Configuration.direction},'IN'));
            expected = 599;
            testCase.verifyEqual(actual,expected, 'The number of input signals does not match expected')
            
            actual = nnz(strcmp({config.Configuration.direction},'OUT'));
            expected = 573;
            testCase.verifyEqual(actual,expected, 'The number of output signals does not match expected')
            
        end
        
        function testReproduceSimplified(testCase)
            
            % [ SETUP ]
            archivename = fullfile(testCase.filefolder_, 'issue025.zip');
            unzip(archivename,pwd)
            
            modelname = 'readfullelac';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
            
            %
            % Add blocks in the model
            %
            configurationblockname = [modelname,'/Configure'];
            configurationblock = add_block('lib_ed247/ED247_Configuration', configurationblockname);
                        
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            
            % [ EXERCISE ]                        
            sim(modelname,'StopTime','0');            
            
            % [ VERIFY ]
            mdlWrks = get_param(modelname,'ModelWorkspace');
            config = mdlWrks.evalin('ED247Configuration');
            
            actual = nnz(strcmp({config.Configuration.direction},'IN'));
            expected = 0;
            testCase.verifyEqual(actual,expected, 'The number of input signals does not match expected')
            
            actual = nnz(strcmp({config.Configuration.direction},'OUT'));
            expected = 148;
            testCase.verifyEqual(actual,expected, 'The number of output signals does not match expected')
            
        end
                
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end