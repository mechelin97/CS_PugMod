#if defined _PugStocks_included
	#endinput
#endif
#define _PugStocks_included

#include <amxmodx>

stock PugMessage(id,sLang[])
{
	if(!id)
	{
		server_print("* %L",LANG_SERVER,sLang);
	}
	
	client_print_color(id,print_team_red,"%s %L",g_sHead,LANG_SERVER,sLang);
	
	return PLUGIN_HANDLED;
}

stock PugRegisterCommand(const sCommand[],const sFunction[],iFlags = 0,const sInfo[]="")
{
	static sBuffer[16];
	formatex(sBuffer,charsmax(sBuffer),".%s",sCommand);
	
	new sDesc[64];
	format(sDesc,charsmax(sDesc),"%L",LANG_SERVER,sInfo);
	
	register_clcmd(sBuffer,sFunction,iFlags,sDesc);
}

stock PugRegisterAdminCommand(const sCommand[],const sFunction[],iFlags = 0,const sInfo[]="")
{
	static sBuffer[16];
	formatex(sBuffer,charsmax(sBuffer),"!%s",sCommand);
	
	new sDesc[64];
	format(sDesc,charsmax(sDesc),"%L",LANG_SERVER,sInfo);
	
	register_clcmd(sBuffer,sFunction,iFlags,sDesc);
}

stock PugGetConfigsDir(sPatch[],iLen)
{
	get_localinfo("amxx_configsdir",sPatch,iLen);
	
	format(sPatch,iLen,"%s/pug",sPatch);
}

stock PugAdminCommand(id,sCommand[],sLang[],iCheck)
{
	if(iCheck)
	{
		new sName[MAX_NAME_LENGTH];
		get_user_name(id,sName,charsmax(sName));
		
		client_print_color(0,print_team_red,"%s %L",g_sHead,LANG_SERVER,sLang,sName,sCommand);
	}
	
	client_print_color(id,print_team_red,"%s %L",g_sHead,LANG_PLAYER,iCheck ? "PUG_CMD_SUCCESS" : "PUG_CMD_FAILURE",sCommand);
}

stock PugAdminCommandClient(id,sCommand[],sLang[],iClient,iCheck)
{
	if(iCheck)
	{
		new sName[MAX_NAME_LENGTH];
		get_user_name(iClient,sName,charsmax(sName));
	
		client_print_color(0,print_team_red,"%s %L",g_sHead,LANG_SERVER,sLang,sName);
	}
	
	client_print_color(id,print_team_red,"%s %L",g_sHead,LANG_PLAYER,iCheck ? "PUG_CMD_SUCCESS" : "PUG_CMD_FAILURE",sCommand);
}

stock PugDisconnect(const id,const sReason[] = "",any:...)
{
	if(sReason[0])
	{
		new sMessage[192];
		vformat(sMessage,charsmax(sMessage),sReason,3);
	
		message_begin(MSG_ONE_UNRELIABLE,SVC_DISCONNECT,_,id);
		write_string(sMessage);
		message_end();
	}
	else
	{
		server_cmd("kick #%i",get_user_userid(id));
	}
}

stock PugGetPercent(iValue,tValue)
{     
	return floatround(floatmul(float(iValue) / float(tValue),100.0));
}

stock PugBuildHelpFile(iLevel,sFileName[],sTag[])
{
	new sDir[64];
	PugGetConfigsDir(sDir,charsmax(sDir));
	format(sDir,charsmax(sDir),"%s/%s",sDir,sFileName);
	
	if(!file_exists(sDir))
	{
		new iCommands = get_concmdsnum(iLevel);
		
		if(iCommands)
		{
			new iFile = fopen(sDir,"wt");
			fputs(iFile,"<style type='text/css'>body{background:#000;margin:2px;color:#FFB000;font-family:Tahoma}</style><table width='100%%'>");
			
			new sCommand[32],sInfo[128],iFlags;
			
			for(new i;i < iCommands;i++)
			{
				get_concmd(i,sCommand,charsmax(sCommand),iFlags,sInfo,charsmax(sInfo),iLevel);
				
				if(sCommand[0] == sTag[0])
				{
					replace_all(sInfo,sizeof(sInfo),"<","&#60;");
					replace_all(sInfo,sizeof(sInfo),">","&#62;");
				
					fprintf(iFile,"<tr><td>%s</td><td>%s</td></tr>",sCommand,sInfo);
				}
			}
			
			fclose(iFile);
		}
	}
}
