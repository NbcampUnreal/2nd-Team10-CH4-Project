using Microsoft.AspNetCore.Mvc;

namespace SFWebAPI.Managers
{
    public class SessionData
    {
        [FromHeader]
        public string? SessionId { get; set; }
    }

    public class SessionManager : Singleton<SessionManager>
    {
        private Dictionary<SessionData, string> sessions = new Dictionary<SessionData, string>();
        public Dictionary<SessionData, string> Sessions
        {
            get { return sessions; }
            set { sessions = value; }
        }

        protected override void OnInit()
        {

        }

        public SessionData GenerateSession()
        {
            var Session = new SessionData();
            Session.SessionId = Guid.NewGuid().ToString();

            return Session;
        }

        public void AddSession(SessionData key, string value)
        {
            sessions.Add(key, value);
        }

        public void RemoveSession(SessionData key)
        {
            if (sessions.ContainsKey(key))
            {
                sessions.Remove(key);
            }
        }

    }
}
