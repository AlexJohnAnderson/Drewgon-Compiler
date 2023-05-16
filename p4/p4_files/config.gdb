set breakpoint pending on
set confirm off
file ./dgc
break drewgon::Err::report
commands
	where
end
break drewgon::InternalError::InternalError
commands
	where
end

define p4
  set args p4_tests/$arg0.dg -n
  run
end
