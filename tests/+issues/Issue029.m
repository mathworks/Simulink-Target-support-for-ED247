classdef (SharedTestFixtures={...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_mock')), ...
        tools.WorkFolderFixture({'ED247','Issue029'})}) ...
        Issue029 < matlab.unittest.TestCase
    % ISSUE029
    %
    % Unable to compile after slrealtime.tlc generation
    %
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function configurationPrerequisites(testCase)
            testCase.assumeFalse(verLessThan('MATLAB','9.9'), ...
                'MATLAB release should be greater or equal to R2020b')
            testCase.assumeNotEmpty(getenv('SLREALTIME_QNX_SP_ROOT'), ...
                'QNX support should be installed')
        end
        
        function setFileFolder(testCase)
            proj = currentProject();
            testCase.filefolder_ = fullfile(proj.RootFolder,'tests','_files');
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testSimpleA429(testCase)
            
            % [ SETUP ]            
            warning('off')
            resetWarnings = onCleanup(@() warning('on'));
            
            archivename = fullfile(testCase.filefolder_, 'A429_simple01.zip');
            unzip(archivename,pwd)
            
            modelname = 'simplea429';
            new_system(modelname)
            load_system(modelname)
            closeModel = onCleanup(@() bdclose(modelname));
                        
            codegenfolder = Simulink.fileGenControl('get','CodeGenFolder');
            % Remove generated files
            genfolder = fullfile(codegenfolder, [modelname,'_slrealtime_rtw']);
            if isfolder(genfolder)
                rmdir(genfolder,'s')
            end
            exefile = fullfile(codegenfolder, [modelname,'.mldatx']);
            if exist(exefile,'file') == 2
                delete(exefile)
            end
            
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
            
            % [ EXERCISE ]
            set_param(modelname,'SolverType','Fixed-Step','SystemTargetFile','slrealtime.tlc','GenCodeOnly','off','LaunchReport','off')
            slbuild(modelname)
            
            % [ VERIFY ]            
            testCase.verifyEqual(exist(exefile,'file'), 2, ...
                sprintf('Expected executable "%s" to be generated', exefile))
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
    end
    
end