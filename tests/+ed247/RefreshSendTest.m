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
                rmdir(testCase.workfolder_,"s")
            end
        end
        
    end
    
    %% TESTS
    methods (Test)
        
        function testA429Send(testCase)
           
            % [ SETUP ]
            currentdir = pwd;
            cd(fullfile(testCase.utilfolder_,'dumper'))
            popd = onCleanup(@() cd(currentdir));
            
            modelname = ['send_a429_r',version('-release')];
            
            ds = Simulink.SimulationData.Dataset();
            
            ds = ds.addElement(timeseries(true(101,1),                  (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I_refresh'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I_refresh');
            ds = ds.addElement(timeseries(uint8((0:100)' + (1:4)),      (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I');
            ds = ds.addElement(timeseries(true(101,1),                  (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I_refresh'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I_refresh');
            ds = ds.addElement(timeseries(uint8((255:-1:155)' + (1:4)), (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I');
            ds = ds.addElement(timeseries(true(101,1),                  (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I_refresh'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I_refresh');
            ds = ds.addElement(timeseries(uint8((150:250)' + (1:4)),    (0:0.1:10)', 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I'), 'Name', 'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I');
            
            in = Simulink.SimulationInput(modelname);
            in = in.setExternalInput(ds);
            
            dumpfilename = [datestr(now,30), '_testA429Send.log'];
                                    
            % [ EXERCISE ]
            parfeval(@() runDumper(testCase,'ecic_func_exchange_a429_uc_tester_simple.xml',dumpfilename),0);
            out = sim(in);
            
            % [ VERIFY ]
            dumpfilename = fullfile(testCase.workfolder_,dumpfilename);
            fid = fopen(dumpfilename,'rt');
            headers = textscan(fid,repmat('%s',1,9),1,'Delimiter',';'); %#ok<NASGU>
            data = textscan(fid,'%d%d%d%d%s%d%d%d%d%s', 'Delimiter', ';');
            fclose(fid);
            %metadata = table(data{1:9},'VariableNames',[headers{:}]);
            data = cellfun(@(x) textscan(x,'%x%x%x%x'),data{end},'UniformOutput',false);
            recv = uint8(cell2mat(vertcat(data{:})));
            
            send = out.logsout.get('T11M4_A429_SIMU2SWIM_BUS_1_350_10_I').Values.Data;
            
            send(cumsum(all(recv(1,:) == send,2))==0,:) = [];
            send(size(recv,1)+1:end,:) = [];
            
            testCase.verifyEqual(recv,send)
            
        end
    
    end
    
    %% PROTECTED METHODS
    methods (Access = protected)
       
        function runDumper(testCase, ecic, dump)
            
            setenv('ECICPATH', fullfile(testCase.filefolder_,'ecic',ecic))
            setenv('DUMPFILE', fullfile(testCase.workfolder_, dump))
            setenv('TIMEOUT', '3000')
            
            !runDumper.sh &
            
        end
        
    end
    
    %% PRIVATE PROPERTIES
    properties (Access = private)
        filefolder_
        utilfolder_
        workfolder_
    end
    
end