set breakpoint pending on
set confirm off
file ./dgc
break drewgon::Err::report
commands
	where
end
break drewgon::ToDoError::ToDoError
commands
	where
end
break drewgon::InternalError::InternalError
commands
	where
end

define p2
  set args p2_tests/$arg0.dg -u
  run
end
