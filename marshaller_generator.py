import re

def LoadFunctionDefs(filename):
	file = open(filename)
	funcs = []
	for line in file:
		line = line.strip()
		if line == '' or line.startswith(('#', '//', '/*', '{', '}', 'namespace')):
			continue

		m = re.match(r'(?P<ret>\w+)\s*(?P<name>\w+)\s*\((?P<args>.*)\)', line)
		(ret, name, argstring) = m.group('ret', 'name', 'args')
		
		args = []
		for _arg in argstring.split(','):
			if _arg == '':
				continue
				
			m = re.match(r'(?P<type>[\w\*\&]+)\s*(?P<param>[\w\*\&]+)', _arg.strip())
			arg = m.group('type', 'param')
			args.append(arg)
			
		func = (ret, name, args)
		funcs.append(func)
	return funcs

def StackType(type):
	map = [ ('Int', ['Type_AI', 'Type_Cog', 'Type_Thing', 'Type_Sector', 'Type_Surface', 'Type_Int', 'Type_Keyframe', 'Type_Material', 'Type_Message', 'Type_Model', 'Type_Sound', 'Type_Template']	),
			('Vector', ['Type_Vector']),
			('Float', ['Type_Float', 'Type_Flex']) ]
			
	for (stack_type, type_list) in map:
		if type in type_list:
			return stack_type

	return ''
	
def OutputMarshallers(funcs, filename):
	f = open(filename, 'w')
	for func in funcs:
		(ret, name, args) = func
		f.write('if( !strcmp( node->lexData.stringVal, \"%s\" ) )\n' % name.lower())
		f.write('{\n')
		
		num = 0
		for arg in args:
			(type, param) = arg
			if type.endswith(('*', '&')):
				type = type[:-1]
			if type == 'Context':
				f.write('    Context& param%i = c;\n' % num)
			elif type == 'Script':
				f.write('    Script* param%i = this;\n' % num)
			else:
				f.write('    %s param%i = c.stack.Pop%s();\n' % (type, num, StackType(type)))
			num += 1
		
		argstring = ', '.join(['param%i' % num for num in range(len(args))])
				
		if(ret != 'Type_Void'):
			f.write('    %s retval = %s( %s );\n' % (ret, name, argstring))
		else:
			f.write('    %s( %s );\n' % (name, argstring))
				
		if ret != 'Type_Void':
			f.write('    if( expression )\n')
			f.write('    {\n')
			
			if ret == 'Type_Variant':
				f.write('        switch( retval.type )\n')
				f.write('        {\n')
				f.write('        case TYPE_AI:\n')
				f.write('        case TYPE_COG:\n')
				f.write('        case TYPE_THING:\n')
				f.write('        case TYPE_SECTOR:\n')
				f.write('        case TYPE_SURFACE:\n')
				f.write('        case TYPE_INT:\n')
				f.write('        case TYPE_KEYFRAME:\n')
				f.write('        case TYPE_MATERIAL:\n')
				f.write('        case TYPE_MESSAGE:\n')
				f.write('        case TYPE_MODEL:\n')
				f.write('        case TYPE_SOUND:\n')
				f.write('        case TYPE_TEMPLATE:\n')
				f.write('            c.stack.PushInt( retval.v_int );\n')
				f.write('            break;\n')
				f.write('        case TYPE_VECTOR:\n')
				f.write('            c.stack.PushVector( retval.v_vector );\n')
				f.write('            break;\n')
				f.write('        case TYPE_FLOAT:\n')
				f.write('        case TYPE_FLEX:\n')
				f.write('            c.stack.PushFloat( retval.v_float );\n')
				f.write('            break;\n')
				f.write('        }\n')
				f.write('        node->type = retval.type;\n')
			else:
				f.write('        c.stack.Push%s( retval );\n' % StackType(ret))
				f.write('        node->type = %s;\n' % ret.upper())
			
			f.write('    }\n')
		f.write('    return;\n')
		f.write('}\n')
		f.write('\n')
	
funcs = LoadFunctionDefs('Engine\\C_Verb.h')
OutputMarshallers(funcs, 'Engine\\C_VerbMarshallers.inc')