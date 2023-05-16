set breakpoint pending on
set confirm off
file ./cmmc
break drewgon::Err::report
commands
	where
end
break drewgon::InternalError::InternalError
commands
	where
end
break drewgon::ToDoError::ToDoError
commands
	where
end

define p5
  set args p5_tests/$arg0.cmm -c
  run
end
