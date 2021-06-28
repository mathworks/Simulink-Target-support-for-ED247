classdef (SharedTestFixtures={...
        tools.WorkFolderFixture({'ED247','Issue025'})}) ...
        Issue026 < matlab.unittest.TestCase
    % ISSUE026
    %
    % MATLAB crash during code generation with slrealtime.tlc
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
            
            set_param(modelname, ...
                'SolverType',       'Fixed-Step', ...
                'SystemTargetFile', 'slrealtime.tlc')
            
            % [ EXERCISE ]
            f = @() slbuild(modelname);
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f)
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end