# ClauLint
Next Project for ClauText, ClauText Schema Maker? and Validator ( check JsonLint )
        # - todo : Tokenizer in clautext?
          # token in 'abc def' with ""
          # -> a b c _ d e f ?
          # chk '_' => __

    ## ClauLint?
    # in data check file?

    # %int %float todo - %quoted_string %string todo - %dateA,B,C?
    # %optional %any # %any is defualt?
    # %one_more  <>  %just_one #chk %just_one is defualt?
    # %id %total_id
    # %required 
    ## %quoted # ' or " 로 둘러 싸인 경우인가? %int + %quoted, %float + %quoted, %dateA, B <- A, C <- B + %quoted.. ??


    $ZERO = 0 # for Event? # define -> $~, USE -> ~

    $Event = { # $Event?, %event_test
      id = test
      $parameter = { input }

      $return = { $COMP>EQ = { $parameter.input /./ZERO } }
    }

    $Event = { # %event_pos_test
      id = pos_test
      $parameter = { input }  # 3 4    $add = { 3 4 }
      $local = { row col }


      $assign_from_ut = { $local.row data = { $parameter.input } dir = { /./row } }
      $assign_from_ut = { $local.col data = { $parameter.input } dir = { /./col } }

      $if = { 
        cond = { $AND_ALL = { $COMP>EQ = { $local.row 0 } $COMP>EQ = { $local.col 0 } 
                  $COMP<EQ = { $local.row 1 } $COMP<EQ = { $local.col 1 } } 
        }
        then = {
          $return = { TRUE }
        }
      }

      # FALSE or ERROR?
      $print = { err_msg = { "row, col is not valid" } }
      $print = { enter = { \n } }

      $return = { FALSE }
    }

    %order_on # order_off, special keyword!

    provinces%required%just_one = { # $가 안 붙으면 파일 체크 데이터?
      %int%required%id%one_more = {
        # Comparison between UserType and UserType?
        pos%required%total_id%event_pos_test = { row = %int col = %int } 
        country%required = %string
        tax%required = %int%event_test # exist - pos, country
        # IS_EXIST? -> $is_item_exist, 2 = { /./a } # 2는 use ToBool4?
        #           -> $is_usertype_exist, 2 = { /./abc }
      }
    }

    countries%required%just_one = {
      %string%required%id%one_more = {
        # x = { 1 2 3 4  } -> x = { %int%one_more%event_is_plus }
        provinces%required = { %required%total_id%one_more%event_test } 
        # %total_id => it is id(unique value) in /./countries/$/provinces ( $ means all ) 
        core%one_more = %total_id%int%event_test
      }
    }


