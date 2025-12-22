export interface SecurityAlert {
  id: string;
  deviceId: string;
  deviceName: string;
  type: 'tamper' | 'attestation_failed' | 'certificate_expired' | 'anomaly';
  severity: 'critical' | 'high' | 'medium' | 'low';
  message: string;
  timestamp: Date;
  resolved: boolean;
}

export interface AuditLogEntry {
  id: string;
  timestamp: Date;
  userId: string;
  userName: string;
  action: string;
  resource: string;
  details: string;
  ipAddress?: string;
}
