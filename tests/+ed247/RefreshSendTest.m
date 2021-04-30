classdef (SharedTestFixtures={ ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files')), ...
        matlab.unittest.fixtures.PathFixture(fullfile(regexprep(mfilename('fullpath'),'\+.*',''),'_files','ecic')), ...
        tools.ParallelPoolFixture('local',2), ...
        }) ...
        RefreshSendTest < matlab.unittest.TestCase
    
    %% CLASS SETUP
    methods (TestClassSetup)
        
        function verifySFunction(testCase)
            testCase.assertEqual(exist('ed247_sfun','file'),3, ...
                'MEX file for S-Function "ed247_sfun" does not exist')
        end
        
        function setFileFolder(testCase)
            proj = simulinkproject();
            testCase.filefolder_ = fullfile(proj.RootFolder,"tests","_files");
        end
        
        function setUtilFolder(testCase)
            proj = simulinkproject();
            testCase.utilfolder_ = fullfile(proj.RootFolder,"tests","_utils");
        end
        
        function setWorkFolder(testCase)
            testCase.workfolder_ = fullfile(tempdir(), "RefreshSendTest");
            if isdir(testCase.workfolder_) %#ok<ISDIR> Backward compatibility
                rmdir(testCase.workfolder_,"s")
            end
            mkdir(testCase.workfolder_)
        end
        
    end
    
    %% CLASS TEARDOWN
    methods (TestClassTeardown)
        
        function cleanWorkFolder(testCase)
            
            if isdir(testCase.workfolder_) %#ok<ISDIR> Backward compatibility
                try
                    rmdir(testCase.workfolder_,"s")
                catch me
                    warning(me.identifier, 'Error while cleanup work folder.\nDetails:\n%s', me.message)
                end
            end
            
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testA429SendSimulation(testCase)
           
            % [ SETUP ]                        
            modelname = ['send_a429_r',version('-release')];
            closeModel = onCleanup(@() bdclose(modelname));
            
            ds = Simulink.SimulationData.Dataset();
            
            enable = true(101,1);
            enable(2:2:end) = false;
            
            ds = ds.addElement(timeseries(enable,                       (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I_refresh'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I_refresh');
            ds = ds.addElement(timeseries(uint8((0:100)' + (1:4)),      (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I');
            ds = ds.addElement(timeseries(enable,                       (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I_refresh'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I_refresh');
            ds = ds.addElement(timeseries(uint8((255:-1:155)' + (1:4)), (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I');
            ds = ds.addElement(timeseries(enable,                       (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I_refresh'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I_refresh');
            ds = ds.addElement(timeseries(uint8((150:250)' + (1:4)),    (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I');
            
            in = Simulink.SimulationInput(modelname);
            in = in.setExternalInput(ds);
            
            dumpfilename = [datestr(now,30), '_testA429Send.log'];
                                    
            % [ EXERCISE ]
            parfeval(@() runDumper(testCase,'ecic_func_exchange_a429_uc_tester_simple.xml',dumpfilename),0);
            out = sim(in);
            
            % [ VERIFY ]
            recv = readDump(testCase, dumpfilename);
                        
            send = out.logsout.get('T11M4_A429_SIMU2SWIM_BUS_1_350_10_I').Values.Data;
            send_refresh = out.logsout.get('T11M4_A429_SIMU2SWIM_BUS_1_350_10_refresh').Values.Data;            
            mask = cumsum(all(recv(1,:) == send,2))==1;            
            send(~mask,:) = [];
            send_refresh(~mask,:) = [];
            send(~send_refresh,:) = [];
            send(size(recv,1)+1:end,:) = [];
            
            testCase.verifyEqual(recv,send)
            
        end
    
        function testA429SendCodeGenerationERT(testCase)
            
            % [ SETUP ]
            modelname = ['send_a429_r',version('-release')];
            if ~bdIsLoaded(modelname)
                load_system(modelname)
                closeModel = onCleanup(@() bdclose(modelname));
            end
            
            set_param(modelname, ...
                'SolverType',       'Fixed-Step', ...
                'SystemTargetFile', 'ert.tlc', ...
                'GenCodeOnly',      'on', ...
                'LaunchReport',     'off')
            
            % [ EXERCISE ]
            f = @() slbuild(modelname);
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f, sprintf('Code generation failed for model "%s"', modelname))
            
            codegenfolder = fullfile(Simulink.fileGenControl('get','CodeGenFolder'), [modelname, '_ert_rtw']);
            hfile = fullfile(codegenfolder,[modelname,'.h']);
            cfile = fullfile(codegenfolder,[modelname,'.c']);
            testCase.assertEqual(exist(hfile,'file'),2, sprintf('Header file does not exist ("%s")', hfile))
            testCase.assertEqual(exist(cfile,'file'),2, sprintf('Source file does not exist ("%s")', cfile))
            
            hcontent = fileread(hfile);
            pattern = regexptranslate('escape','extern char configurationFile[256];');
            testCase.verifyMatches(hcontent, pattern, ...
                sprintf('Header file ("%s") should contains configurationFile declaration', hfile))
            
            ccontent = fileread(cfile);
            
            pattern = regexptranslate('escape',sprintf('static IO_t *%s_io;',modelname));
            testCase.verifyMatches(ccontent, pattern, ...
                sprintf('Source file ("%s") should contains IO variable declaration', cfile))
                                    
        end
        
        function testA429SendExecutableERT(testCase)
            
            % [ SETUP ]
            modelname = ['send_a429_r',version('-release')];
            if ~bdIsLoaded(modelname)
                load_system(modelname)
                closeModel = onCleanup(@() bdclose(modelname));
            end
            
            set_param(modelname, ...
                'SolverType',               'Fixed-Step', ...
                'SystemTargetFile',         'ert.tlc', ...
                'GenCodeOnly',              'off', ...
                'LaunchReport',             'off', ...
                'GenerateSampleERTMain',    'off', ...
                'ERTCustomFileTemplate',    'testA429SendExecutableERT_CFP.tlc')
            
            dumpfilename = [datestr(now,30), '_testA429SendExecutableERT.log'];
            
            % [ EXERCISE ]
            f = @() slbuild(modelname);
            
            % [ VERIFY ]
            testCase.verifyWarningFree(f, sprintf('Code generation failed for model "%s"', modelname))
                 
            exefile = fullfile(Simulink.fileGenControl('get','CodeGenFolder'), [modelname, '.exe']);
            testCase.assertEqual(exist(exefile,'file'),2, ...
                sprintf('EXE "%s" should have been generated', exefile))
            
            parfeval(@() runDumper(testCase,'ecic_func_exchange_a429_uc_tester_simple.xml',dumpfilename),0);
            pause(0.5)
            testCase.runExe([modelname, '.exe'])
            
            recv = readDump(testCase, dumpfilename);
            
        end
        
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
       
        function runDumper(testCase, ecic, dump)
            
            currentdir = pwd;
            cd(fullfile(testCase.utilfolder_,'dumper'))
            popd = onCleanup(@() cd(currentdir));
            
            setenv('ECICPATH', fullfile(testCase.filefolder_,'ecic',ecic))
            setenv('DUMPFILE', fullfile(testCase.workfolder_, dump))
            setenv('TIMEOUT', '3000')
            
            !runDumper.sh
            
        end
        
        function recv = readDump(testCase, dumpfilename)
           
            fid = fopen(fullfile(testCase.workfolder_,dumpfilename),'rt');
            headers = textscan(fid,repmat('%s',1,9),1,'Delimiter',';'); %#ok<NASGU>
            data = textscan(fid,'%*d%*d%*d%*d%*s%*d%*d%*d%*d%s', 'Delimiter', ';');
            fclose(fid);
            %metadata = table(data{1:9},'VariableNames',[headers{:}]);
            testCase.assertNotEmpty(data, 'No data registered in dump file')
            data = cellfun(@(x) hex2dec(strsplit(x,' '))',data{end},'UniformOutput',false);
            recv = uint8(vertcat(data{:}));
            testCase.assertNotEmpty(recv, 'No received data')
            
        end
        
        function varargout = runExe(testCase, exename, varargin)
           
            currentdir = pwd;
            codegenfolder = Simulink.fileGenControl('get','CodeGenFolder');
            cd(codegenfolder)
            c = onCleanup(@() cd(currentdir));
            
            if ~isempty(varargin)
                logfile = varargin{1};
                retainlog = true;
            else
                logfile = 'tmp.log';
                retainlog = false;
            end
            
            config = ed247.Configuration.default();
            libpath = strjoin(config.SystemPath,':');
            libpath = strrep(libpath,'\','/');
            libpath = strrep(libpath,'C:','/c');
            
            setenv('LIBPATH', libpath)
            setenv('EXENAME', exename)
            setenv('LOGFILE', logfile)
                                    
            [status,log] = system(fullfile(testCase.utilfolder_,'runExe.sh'));
            testCase.assertEqual(status,0, sprintf('Failed to execute simulation with message:\n%s',log))
            
            logtxt = fileread(logfile);
            if nargout
                varargout = {logtxt};
            end
            
            if ~retainlog
                delete(logfile)
            end
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
        utilfolder_
        workfolder_
    end
    
end