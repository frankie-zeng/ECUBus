list(APPEND incs "." "inc_common" "config" "inc_services" "inc_custom")

kdf_component_register(INCLUDE_DIRS ${incs}
					  )
