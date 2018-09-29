# ClauLint
        Next Project for ClauText, ClauText Schema Maker? and Validator ( check JsonLint )
        With ClauText API?
# ToDo
        1. DONE - %multiple
        2. %can_empty (array)
        
        3. DONE? - id
        4. DONE? - total_id
                (do not check total_id, if depth == 0)?? 
                (check id, if depth == 0) !?
        5. one_more 
        6. just_one
                (5,6 => $list_size = { }) ?
  
# Test
        # done? - %int%string -> int or string!
        #cf) %int%multiple or %int%multiple_4


        # remove - just_one, one_more, order_off? or %multiple_on ?
        # %int + number -> %int %int %int ...
        # array -> %order_on?
        # object -> %order_off?
        # optional! - remove, and use defualt? <> required..
        # num of valid var or val??
        # check - exist chck array?

        %order_on # default, <> use order_off

        # object + order_off, array + order_on. ?

        $test2 = {

        }

        $Event = {
                id = __init__

                $load_only_data2 = { { "C:\Users\vztpv\Desktop\Clau\ClauLint\ClauLint\test2.txt" } 
                { test2 } { 0 } { 0 } }

                $print = { data = { "init end.." } }
                $print = { enter = { \n } }

                $return = { test2 } # returns data folder?
        }

        $Event = {
                id = aaa

                $parameter = { name is_usertype real_dir } # real_dir - ToDo! 

                $print = { { $parameter.is_usertype } }
                $print = { { \n } }
                $print = { { $parameter.name } }
                $print = { { \n } }
                $print = { { $parameter.real_dir } }
                $print = { { \n } }

                $if = { 
                        cond = {
                                TRUE
                        }
                        then = {
                                $return = { TRUE }
                        }
                }

                $return = { FALSE }
        }

        xxx%optional = %int
        yyy = 3

        x = { # %multiple_on + one ut. and repeating this can`t.
                a%can_empty_ut%one_more%optional = { %order_on %multiple_on # - default, remove multiple_on, off?
                        %can_empty_ut%int%event_aaa = { %order_on %multiple_on %int%total_id }
                }  
                                # cf) number of int?
        }

# Test2
        #xxx = 100
        yyy = 3
        #k = { 3 }
        x = {	
                #a = { 3 = { } 4 = { 5 8 } 4 = { 6  } } 
                #a = { 44 = { 1 } }
                #a = { 445 = { 3 7 } }
        }

