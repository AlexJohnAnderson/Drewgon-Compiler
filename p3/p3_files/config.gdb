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

define p3
  set args p3_tests/$arg0.dg -p --
  run
end
