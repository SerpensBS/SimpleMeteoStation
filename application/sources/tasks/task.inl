namespace Application
{
	template<typename PT>
	Middleware::ReturnCode Task<PT>::Execute()
	{
		auto status = run_function_(function_input_parameter_);

		if (Middleware::ReturnCode::OK != status && error_callback_func_)
		{
			error_callback_func_(function_input_parameter_);
		}
		else if (Middleware::ReturnCode::OK == status && success_callback_func_)
		{
			success_callback_func_(function_input_parameter_);
		}

		return status;
	}
}