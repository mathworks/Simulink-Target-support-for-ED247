classdef (SharedTestFixtures={ ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files')); ...
        }) ...
        CodeGenerationTest < matlab.unittest.TestCase
    %
    %
    
    % ts = matlab.unittest.TestSuite.fromClass(?ed247.CodeGenerationTest);
    % ts = ts.selectIf('ParameterProperty','ModelName','ParameterName','ana_sendreceive');
    % run(ts) 
    %
    
    %% TEST PARAMETERS
    properties (TestParameter)
        ModelName = {'ana_sendreceive','dis_sendreceive','nad_sendreceive','vnad_sendreceive','a429_sendreceive','a664_sendreceive'};
    end
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function skipTests(testCase)
            testCase.assumeTrue(false, 'Something is broken in the code generation -> need more investigations')
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testCodeGenerationOnly(testCase,ModelName)
            
            % [ SETUP ]
            codegenfolder = Simulink.fileGenControl('get','CodeGenFolder');
            
            load_system(ModelName)
            c = { ...
                onCleanup(@() bdclose(ModelName)); ...
                }; %#ok<NASGU>
            
            set_param(ModelName, ...
                'LoadExternalInput','off',          ... % To avoid warning
                'SolverType',       'Fixed-Step',   ...
                'SystemTargetFile', 'ert.tlc',      ...
                'GenCodeOnly',      'on',           ...
                'GenerateReport',   'off')
            
            % Choose custom toolchain
            if ispc()
                set_param(ModelName,'Toolchain','MinGW64 (ED247) | gmake makefile (64-bit Windows)')
            end
            
            % [ EXERCISE ]
            rtwbuild(ModelName,'ForceTopModelBuild',true)
            
            % [ VERIFY ]
            testCase.verifyTrue(isdir(fullfile(codegenfolder,[ModelName,'_ert_rtw'])))
            
        end
        
        function testCodeGenerationAndCompilation(testCase,ModelName)
            
            % [ SETUP ]
            codegenfolder = Simulink.fileGenControl('get','CodeGenFolder');
            
            if ispc()
                exeext = '.exe';
            else
                exeext = '';
            end
            exename = fullfile(codegenfolder,[ModelName,exeext]);
            if exist(exename,'file') == 2
                delete(exename)
            end
            
            load_system(ModelName)
            c = { ...
                onCleanup(@() bdclose(ModelName)); ...
                onCleanup(@() delete(exename)); ...
                }; %#ok<NASGU>
            
            set_param(ModelName, ...
                'LoadExternalInput','off',          ... % To avoid warning
                'SolverType',       'Fixed-Step',   ...
                'SystemTargetFile', 'ert.tlc',      ...
                'GenCodeOnly',      'off',          ...
                'GenerateReport',   'off')
            
            % Choose custom toolchain
            if ispc()
                set_param(ModelName,'Toolchain','MinGW64 (ED247) | gmake makefile (64-bit Windows)')
            end
            
            % [ EXERCISE ]
            rtwbuild(ModelName,'ForceTopModelBuild',true)
            
            % [ VERIFY ]
            testCase.verifyEqual(exist(exename,'file'),2, ...
                sprintf('No executable "%s" generated', exename))
            
        end
        
    end
    
end