package tweek;

/**
 *	Generated from IDL definition of interface "Observer"
 *	@author JacORB IDL compiler 
 */


public abstract class ObserverPOA
	extends org.omg.PortableServer.Servant
	implements org.omg.CORBA.portable.InvokeHandler, tweek.ObserverOperations
{
	static private final java.util.Hashtable m_opsHash = new java.util.Hashtable();
	static
	{
		m_opsHash.put ( "update", new java.lang.Integer(0));
	}
	private String[] ids = {"IDL:tweek/Observer:1.0","IDL:omg.org/CORBA/Object:1.0"};
	public tweek.Observer _this()
	{
		return tweek.ObserverHelper.narrow(_this_object());
	}
	public tweek.Observer _this(org.omg.CORBA.ORB orb)
	{
		return tweek.ObserverHelper.narrow(_this_object(orb));
	}
	public org.omg.CORBA.portable.OutputStream _invoke(String method, org.omg.CORBA.portable.InputStream _input, org.omg.CORBA.portable.ResponseHandler handler)
		throws org.omg.CORBA.SystemException
	{
		org.omg.CORBA.portable.OutputStream _out = null;
		// do something
		// quick lookup of operation
		java.lang.Integer opsIndex = (java.lang.Integer)m_opsHash.get ( method );
		if ( null == opsIndex )
			throw new org.omg.CORBA.BAD_OPERATION(method + " not found");
		switch ( opsIndex.intValue() )
		{
			case 0: // update
			{
				_out = handler.createReply();
				update();
				break;
			}
		}
		return _out;
	}

	public String[] _all_interfaces(org.omg.PortableServer.POA poa, byte[] obj_id)
	{
		return ids;
	}
}
