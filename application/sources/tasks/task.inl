namespace Application
{
	template<typename PT>
	Middleware::ReturnCode Task<PT>::Execute()
	{
		auto status = run_function(function_input_parameter);

		if (Middleware::ReturnCode::OK != status && error_callback_func)
		{
			error_callback_func(function_input_parameter);
		}
		else if (Middleware::ReturnCode::OK == status && success_callback_func)
		{
			success_callback_func(function_input_parameter);
		}

		return status;
	}
}