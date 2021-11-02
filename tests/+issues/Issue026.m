classdef (SharedTestFixtures={...
        tools.WorkFolderFixture({'ED247','Issue026'})}) ...
        Issue026 < matlab.unittest.TestCase
    % ISSUE026
    %
    % MATLAB crash during code generation with slrealtime.tlc
    %
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function enableOnWindowsOnly(testCase)
            testCase.assumeTrue(ispc(), "slrealtime.tlc is compatible on Windows only")
        end
        
        function setFileFolder(testCase)
            proj = currentProject();
            testCase.filefolder_ = fullfile(proj.RootFolder,'tests','_files');
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testReadELACFullReceive(testCase)
            %
            % MATLAB crash during code generation
            %
                        
            % [ SETUP ]
            warning('off') % Lots of warnings due to unconnected lines (do not care for this test)
            restoreWarnings = onCleanup(@() warning('on'));
            
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
            
            receiveblockname = [modelname,'/Receive'];
            receiveblock = add_block('lib_ed247/ED247_Receive', receiveblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            set(receiveblock, 'enable_refresh', 'on', 'show_port_labels', 'on')
            
            set_param(modelname, ...
                'SolverType',       'Fixed-Step', ...
                'SystemTargetFile', 'slrealtime.tlc')
            
            % [ EXERCISE ]            
            f = @() slbuild(modelname);
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f)
            
        end
        
        function testReadELACFullSend(testCase)
            %
            % MATLAB crash during code generation
            %
                        
            % [ SETUP ]
            warning('off') % Lots of warnings due to unconnected lines (do not care for this test)
            restoreWarnings = onCleanup(@() warning('on'));
            
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
            
            sendblockname = [modelname,'/Send'];
            sendblock = add_block('lib_ed247/ED247_Send', sendblockname);
            
            %
            % Configure blocks
            %   - configuration file
            %   - Enable refresh
            %
            set(configurationblock, 'configurationFilename', '''ELACe2C_ECIC.xml''')
            set(sendblock, 'enable_refresh', 'on', 'show_port_labels', 'on')
            
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