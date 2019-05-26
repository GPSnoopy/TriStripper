
#ifndef HEADER_GUARD_STATIC_ASSERT_H
#define HEADER_GUARD_STATIC_ASSERT_H



namespace _static_assert {

	template <bool> class compile_time_error;
	template <> class compile_time_error<true> { };

}



#define static_assert(expr)				{ _static_assert::compile_time_error<((expr) != 0)> ERROR_STATIC_ASSERT; (void) ERROR_STATIC_ASSERT; }
#define static_assert_msg(expr, msg)	{ _static_assert::compile_time_error<((expr) != 0)> ERROR_##msg; (void) ERROR_##msg; }



#endif // HEADER_GUARD_STATIC_ASSERT_H
