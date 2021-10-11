classdef (SharedTestFixtures={ ...
        matlab.unittest.fixtures.PathFixture(fullfile(fileparts(fileparts(mfilename('fullpath'))),'_files','ecic')), ...        
        }) ...
        ED247MexTest < matlab.unittest.TestCase
   
    %% TESTS
    methods (Test)
        
        function testA429MainSimple(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_a429_uc_main_simple.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'T11M4_A429_SIMU2SWIM_BUS_1_350_10_I';    'T11M4_A429_SIMU2SWIM_BUS_1_200_10_I';  'T11M4_A429_SIMU2SWIM_BUS_1_200_11_I'   }, ...
                "direction",        { 'OUT';                                    'OUT';                                  'OUT'                                   }, ...
                "type",             { 'uint8';                                  'uint8';                                'uint8'                                 }, ...
                "dimensions",       { 1;                                        1;                                      1                                       }, ...
                "width",            { 4;                                        4;                                      4                                       }, ...
                "size",             { 4;                                        4;                                      4                                       }, ...
                "sample_time",      { 1;                                        1;                                      1                                       }, ...
                "validity_duration",{ 1;                                        1;                                      1                                       }, ...
                "signal_type",      { 'A429';                                   'A429';                                 'A429'                                  }, ...
                "sample_size",      { 4;                                        4;                                      4                                       }  ...
                );
            testCase.verifyEqual(actual,expected)
            
            expected = { ...
                'icd_func_exchange_a429_uc_main_simple.xml'; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testA429TesterSimple(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_a429_uc_tester_simple.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'T11M4_A429_SIMU2SWIM_BUS_1_350_10';      'T11M4_A429_SIMU2SWIM_BUS_1_200_10';    'T11M4_A429_SIMU2SWIM_BUS_1_200_11'     }, ...
                "direction",        { 'IN';                                     'IN';                                   'IN'                                    }, ...
                "type",             { 'uint8';                                  'uint8';                                'uint8'                                 }, ...
                "dimensions",       { 1;                                        1;                                      1                                       }, ...
                "width",            { 4;                                        4;                                      4                                       }, ...
                "size",             { 4;                                        4;                                      4                                       }, ...
                "sample_time",      { 1;                                        1;                                      1                                       }, ...
                "validity_duration",{ 1;                                        1;                                      1                                       }, ...
                "signal_type",      { 'A429';                                   'A429';                                 'A429'                                  }, ...
                "sample_size",      { 4;                                        4;                                      4                                       }  ...
                );
            testCase.verifyEqual(actual,expected)
            
            expected = { ...
                'icd_func_exchange_a429_uc_tester_simple.xml'; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testA664Main(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_a664_uc_main.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'T11MX_19492_WAIT_STEP_I' }, ...
                "direction",        { 'OUT';                    }, ...
                "type",             { 'uint8';                  }, ...
                "dimensions",       { 1;                        }, ...
                "width",            { 4;                        }, ...
                "size",             { 4;                        }, ...
                "sample_time",      { 0.064;                    }, ...
                "validity_duration",{ 0.064;                    }, ...
                "signal_type",      { 'A664';                   }, ...
                "sample_size",      { 4;                        }  ...
                );
            testCase.verifyEqual(actual,expected, 'AbsTol', 1e-6)
            
            expected = { ...
                'icd_func_exchange_a664_uc_main.xml'; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testA664Tester(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_a664_uc_tester.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'T11MX_15430_TEST_STEP_O' }, ...
                "direction",        { 'IN';                     }, ...
                "type",             { 'uint8';                  }, ...
                "dimensions",       { 1;                        }, ...
                "width",            { 4;                        }, ...
                "size",             { 4;                        }, ...
                "sample_time",      { 1;                        }, ...
                "validity_duration",{ 1;                        }, ...
                "signal_type",      { 'A664';                   }, ...
                "sample_size",      { 4;                        }  ...
                );
            testCase.verifyEqual(actual,expected, 'AbsTol', 1e-6)
            
            expected = { ...
                'icd_func_exchange_a664_uc_tester.xml'; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testA825Main(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_a825_uc_main.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'Stream0';	'Stream1';  'Stream0';	'Stream1';    }, ...
                "direction",        { 'IN';         'IN';       'OUT';      'OUT';        }, ...
                "type",             { 'uint8';      'uint8';    'uint8';	'uint8';      }, ...
                "dimensions",       { 1;            1;          1;            1;          }, ...
                "width",            { 3;            5;          3;            5;          }, ...
                "size",             { 3;            5;          3;            5;          }, ...
                "sample_time",      { 1;            0.0640;     1;            0.0640;     }, ...
                "validity_duration",{ 1;            0.0640;     1;            0.0640;     }, ...
                "signal_type",      { 'A825';       'A825';     'A825';       'A825';     }, ...
                "sample_size",      { 3;            5;          3;            5;          }  ...
                );
            testCase.verifyEqual(actual,expected, 'AbsTol', 1e-6)
            
            expected = { ...
                'icd_func_exchange_a825_uc_main.xml'; ...
                'icd_func_exchange_a825_uc_main.xml'; ...
                'icd_func_exchange_a825_uc_main.xml'; ...
                'icd_func_exchange_a825_uc_main.xml'; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testA825Tester(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_a825_uc_tester.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'Stream0';	'Stream1';  'Stream0';	'Stream1';    }, ...
                "direction",        { 'IN';         'IN';       'OUT';      'OUT';        }, ...
                "type",             { 'uint8';      'uint8';    'uint8';	'uint8';      }, ...
                "dimensions",       { 1;            1;          1;            1;          }, ...
                "width",            { 3;            5;          3;            5;          }, ...
                "size",             { 3;            5;          3;            5;          }, ...
                "sample_time",      { 1;            0.0640;     1;            0.0640;     }, ...
                "validity_duration",{ 1;            0.0640;     1;            0.0640;     }, ...
                "signal_type",      { 'A825';       'A825';     'A825';       'A825';     }, ...
                "sample_size",      { 3;            5;          3;            5;          }  ...
                );
            testCase.verifyEqual(actual,expected, 'AbsTol', 1e-6)
            
            expected = { ...
                'icd_func_exchange_a825_uc_tester.xml'; ...
                'icd_func_exchange_a825_uc_tester.xml'; ...
                'icd_func_exchange_a825_uc_tester.xml'; ...
                'icd_func_exchange_a825_uc_tester.xml'; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testNADMain(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_nad_uc_main.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'Signal00';	'Signal01';     'Signal10';     'Signal11';	}, ...
                "direction",        { 'OUT';        'OUT';          'OUT';          'OUT';      }, ...
                "type",             { 'uint8';      'uint8';        'uint8';        'single';   }, ...
                "dimensions",       { 1;            1;              1;              2;          }, ...
                "width",            { 1;            1;              1;              4;          }, ...
                "size",             { 1;            1;              1;              [2,2];      }, ...
                "sample_time",      { 0.01;         0.01;           0.01;           0.01;       }, ...
                "validity_duration",{ 0;            0;              0;              0;          }, ...
                "signal_type",      { 'NAD';       'NAD';           'NAD';          'NAD';      }, ...
                "sample_size",      { 1;            1;              1;              16;         }  ...
                );
            testCase.verifyEqual(actual,expected, 'AbsTol', 1e-6)
            
            expected = { ...
                ''; ...
                ''; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
        function testNADTester(testCase)
           
            % [ SETUP ]
            filename = which("ecic_func_exchange_nad_uc_tester.xml");
            
            % [ EXERCISE ]
            [actual,files] = ed247_mex(char(filename));
            
            % [ VERIFY ]
            expected = struct( ...
                "name",             { 'Signal00';	'Signal01';     'Signal11';	}, ...
                "direction",        { 'IN';         'IN';           'IN';       }, ...
                "type",             { 'uint8';      'uint8';        'single';   }, ...
                "dimensions",       { 1;            1;              2;          }, ...
                "width",            { 1;            1;              4;          }, ...
                "size",             { 1;            1;              [2,2];      }, ...
                "sample_time",      { 0.01;         0.01;           0.01;       }, ...
                "validity_duration",{ 0;            0;              0;          }, ...
                "signal_type",      { 'NAD';       'NAD';           'NAD';      }, ...
                "sample_size",      { 1;            1;              16;         }  ...
                );
            testCase.verifyEqual(actual,expected, 'AbsTol', 1e-6)
            
            expected = { ...
                ''; ...
                ''; ...
                };
            [~,name,ext] = cellfun(@fileparts,files,'UniformOutput',false);
            actual = cellfun(@horzcat, name,ext,'UniformOutput',false);
            testCase.verifyEqual(actual,expected)
            
        end
        
    end
    
end