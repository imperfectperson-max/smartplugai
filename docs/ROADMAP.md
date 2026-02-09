# Smart Plug AI Roadmap

This roadmap outlines the phased development plan for Smart Plug AI, with a **security-first approach** from Day 1. The project is structured in three phases over 12 months, with detailed week-by-week milestones for Phase 1.

## 🎯 Vision

Build South Africa's most secure IoT platform for smart plug systems that provides real-time power monitoring, intelligent insights, and seamless user experience across mobile and web platforms, with bank-grade security built-in from the ground up.

---

## 📊 Phase Overview

| Phase | Timeline | Budget | Focus |
|-------|----------|--------|-------|
| **Phase 1** | Now - 7 weeks | R5,000 | Secure MVP Development |
| **Phase 2** | Weeks 8-20 (13 weeks) | R3,000,000 | Pilot & Refinement |
| **Phase 3** | Weeks 21+ | R6,000,000 | Commercial Launch |

---

## 📅 Phase 1: Secure MVP Development (Now - 7 Weeks)

**Goal**: Build a working secure prototype with 3 devices, mobile app with 2FA, and secure cloud backend.

**Team**: 3-4 people working in parallel on hardware, firmware, backend, and mobile integration.

**Efficiency Note**: Compressed from 16 weeks through parallel workstreams and pre-completed mobile/web mock implementations (see `PRE_HARDWARE_COMPLETE.md`).

### Week-by-Week Implementation Plan

#### **Week 1-2: Hardware Assembly & Basic Firmware**
- [ ] Secure breadboard setup (3 plugs)
- [ ] ESP32-S3 + ATECC608A provisioning
  - Generate unique device IDs
  - Provision ATECC608A with device private keys (ECDSA P256)
  - Lock ATECC608A configuration (irreversible)
- [ ] Enable secure boot + flash encryption on ESP32-S3
- [ ] Relay control via GPIO with optoisolation (PC817)
- [ ] SCT-013 calibration with 33Ω burden resistors
- [ ] Basic MQTT publish over TLS 1.3
- [ ] Tamper detection circuit testing (MAX6316 watchdog)
- [ ] Encrypted configuration storage (24C256 EEPROM)

**Team Split**: 2 people on hardware assembly, 1-2 on firmware infrastructure

**Detailed Guide**: See `docs/WEEK1_HARDWARE_MASTERY.md` for day-by-day instructions

**Deliverable**: 3 working breadboard prototypes with hardware security enabled

#### **Week 3: Mobile App Integration**
- [ ] Replace mock services with real Firebase backend
- [ ] Implement secure device pairing via QR code + challenge-response authentication
- [ ] Real-time power display with encrypted TLS communications
- [ ] Manual ON/OFF toggle with signed ECDSA commands for device control
- [ ] Encrypted historical data display (24 hours)
- [ ] Device attestation status integration
- [ ] Tamper alert notifications

**Team Split**: 2 on mobile integration, 1-2 on backend preparation

**Deliverable**: Mobile app (iOS + Android) with secure pairing and real-time control

#### **Week 4: Cloud Backend V1**
- [ ] Firebase Firestore with field-level encryption (AES-256-GCM)
- [ ] Cloud Functions with input validation and rate limiting
- [ ] User management with role-based access control (RBAC)
- [ ] WebSocket over TLS for real-time updates
- [ ] Audit logging for all operations
- [ ] MQTT broker over TLS 1.3 with client certificates
- [ ] Signed command generation and verification (ECDSA)
- [ ] Device registration and attestation endpoints

**Team Split**: 2-3 on backend implementation, 1 on firmware integration

**Deliverable**: Secure cloud backend with encrypted data storage and signed commands

#### **Week 5: Safety & Enclosure**
- [ ] PCB design with security zones (isolated AC/DC circuits)
- [ ] 3D-printed enclosure v1 (tamper-resistant with MAX6316 integration)
- [ ] Mains wiring with fuse + MOV protection (3A quick-blow + 275V varistor)
- [ ] Temperature monitoring safety cut-off
- [ ] Overload detection (15A limit)
- [ ] Physical security (anti-tamper screws, void-if-removed seals)
- [ ] Secure assembly process documentation

**Team Split**: 2 on hardware/enclosure design, 2 on continued backend work

**Deliverable**: 3 enclosed secure prototypes with safety certifications ready

#### **Week 6: Rule Engine & Automation**
- [ ] Time-based schedules (signed commands with timestamps)
- [ ] Manual rules via app (with user authentication + 2FA)
- [ ] Encrypted energy usage alerts
- [ ] Load-shedding detection (static schedule with secure updates)
- [ ] Rate limiting on control commands (10 commands/min per device)
- [ ] Command replay attack prevention (nonce + timestamp verification)

**Team**: All hands on integration testing

**Deliverable**: Secure automation and scheduling system

#### **Week 7: Pilot Testing & Security Audit**
- [ ] 10 devices deployed (3 real plugs + 7 virtual test devices)
- [ ] Pilot with friends/family (controlled security environment)
- [ ] Independent security penetration testing
- [ ] Bug collection & fixing (security-first priority)
- [ ] UI/UX improvements based on feedback
- [ ] Basic data collection for ML training (anonymized and encrypted)
- [ ] Security audit report generation
- [ ] Documentation of security findings and remediation

**Team**: All hands on testing and documentation

**Deliverable**: Pilot-tested system with security audit results and improvements

---

## 📅 Phase 2: Pilot & Refinement (Weeks 8-20)

**Goal**: Expand pilot to 500 devices with security monitoring, achieve SABS/ICASA compliance, and complete first production batch.

**Budget**: R3,000,000
- Manufacturing (5,000 secure units): R1,500,000
- Certifications (SABS, ICASA, security audits): R600,000
- Cloud infrastructure scaling (TLS, CDN, monitoring): R500,000
- Security team & tools: R300,000
- Marketing & pilot program: R100,000

### Week 8-10: Secure Manufacturing Prep
- [ ] PCB v2 design with security zones and improved layout
- [ ] Component sourcing with chain of custody verification
- [ ] Safety certification testing (SABS compliance)
- [ ] Independent security audit (penetration testing)
- [ ] Secure packaging (tamper-evident seals and documentation)
- [ ] Secure assembly process documentation
- [ ] Quality assurance testing (100% security validation)

### Week 11-13: Secure Software V2.0
- [ ] Energy disaggregation v1 (on-device, encrypted)
- [ ] Daily/weekly energy reports (encrypted in transit and at rest)
- [ ] Cost tracking with PCI DSS compliance considerations
- [ ] Tariff integration (prepaid vs postpaid with secure billing)
- [ ] Export reports with digital signatures (PDF/CSV)
- [ ] Enhanced device attestation (firmware integrity checks)
- [ ] Secure OTA updates with code signing (RSA-3072)

### Week 14-16: Secure Pilot Expansion
- [ ] 500-device pilot program with security monitoring
- [ ] Customer support system (Zendesk with SSO)
- [ ] Security Operations Center (SOC) dashboard
- [ ] Automated security scanning (CodeQL, dependency checks)
- [ ] Mobile app v2 (app store ready with security review)
- [ ] Bug bounty program launch (R5k-R50k rewards)

### Week 17-20: Certification & Hardening
- [ ] SABS certification completion
- [ ] ICASA type approval submission
- [ ] First penetration testing results and remediation
- [ ] Field-level encryption implementation
- [ ] Rate limiting and DDoS protection
- [ ] Compliance documentation (POPIA readiness)
- [ ] Security incident response procedures

**Milestone**: 500 devices deployed, SABS/ICASA certified, security-hardened platform

---

## 📅 Phase 3: Commercial Launch (Weeks 21+)

**Goal**: Scale to 10,000+ devices with nationwide retail launch, SOC2 Type I audit, and enterprise features.

**Budget**: R6,000,000
- Inventory (50,000 units): R3,000,000
- Retail partnerships & marketing: R1,500,000
- SOC2 Type I preparation & audit: R500,000
- Team expansion (20 people including security): R800,000
- Infrastructure & scaling: R200,000

### Week 21-24: Retail Launch Preparation
- [ ] Retail partnerships (Takealot, Builders, Makro)
- [ ] Marketing campaign with security messaging
- [ ] Enterprise sales team
- [ ] SOC2 Type I controls implementation
- [ ] 24/7 security monitoring
- [ ] Advanced threat detection

### Week 25-28: Nationwide Launch
- [ ] Public launch event
- [ ] Social media campaign
- [ ] Press releases emphasizing security
- [ ] First 10,000 units shipped
- [ ] Customer onboarding with security training
- [ ] Retail presence in major cities

### Week 29-32: Enterprise Features
- [ ] Multi-tenant architecture
- [ ] Advanced RBAC (custom roles)
- [ ] Enterprise API with rate limiting
- [ ] White-label options
- [ ] Integration with building management systems
- [ ] Compliance reports for enterprises

### Week 33-36: SOC2 & Scale
- [ ] SOC2 Type I audit completion
- [ ] Security certification showcase
- [ ] Scale to 50,000 devices capacity
- [ ] International expansion planning (GDPR compliance)
- [ ] Advanced analytics and AI features
- [ ] Quantum-resistant cryptography research

**Milestone**: Nationwide presence, 10,000+ active devices, SOC2 Type I certified, R1.5M+ monthly revenue

---

## 🔮 Future Vision (Weeks 37+)

### Phase 4: Enterprise Expansion (Weeks 37-60)
- **SOC2 Type II certification**
- **ISO 27001 certification**
- **Zero-trust architecture implementation**
- **Multi-region deployment** (3+ African countries)
- **Advanced AI-powered threat detection**
- **100,000+ active devices**

### Phase 5: Regional Expansion (Weeks 61-96)
- **GDPR compliance** for European markets
- **Supply chain security** (blockchain audit trails)
- **Partnerships with utility companies**
- **Virtual power plant participation**
- **1,000,000+ devices globally**

### Phase 6: Technology Leadership (Weeks 97-144)
- **Quantum-resistant cryptography** implementation
- **Industry security standards** contribution
- **Open-source security framework**
- **AI appliance recognition** by power signature
- **Grid services** and demand response
- **Global market leadership**

---

## 🎪 Key Success Metrics

### Technical Metrics
- **Uptime**: 99.9%+ for cloud services (with 24/7 security monitoring)
- **Latency**: <200ms for real-time data updates over TLS
- **Accuracy**: ±1% for power measurements (calibrated with ATECC608A)
- **Security**: Zero security breaches, 100% device attestation pass rate
- **Encryption**: 100% of sensitive data encrypted in transit and at rest

### Product Metrics
- **User retention**: 80%+ after 30 days (trust through security)
- **Daily active users**: 70%+ of registered users
- **App rating**: 4.7+ stars (security as differentiator)
- **NPS (Net Promoter Score)**: 60+ (security trust factor)
- **Device attestation**: 99.9%+ devices passing integrity checks

### Development Metrics
- **Code coverage**: 85%+ for critical security paths
- **Deployment frequency**: 3+ per week (with security scans)
- **Mean time to recovery**: <30 minutes for critical security issues
- **Bug resolution time**: <24 hours for security vulnerabilities
- **Security scan**: 100% of deployments scanned with CodeQL

### Security Metrics (New)
- **Penetration test results**: 0 critical vulnerabilities
- **Security audit score**: 95%+ compliance
- **Incident response time**: <15 minutes detection, <1 hour mitigation
- **Device tamper detection**: 100% alert success rate
- **Certificate management**: 0 expired certificates
- **2FA adoption**: 90%+ of users enabled

### Business Metrics (New)
- **Security premium**: 10-15% higher pricing vs non-secure competitors
- **Enterprise adoption**: 50+ corporate clients (SOC2 certified)
- **Compliance certifications**: SABS, ICASA, SOC2 Type II, ISO 27001
- **Churn rate**: <5% monthly (vs 15% industry average)
- **Customer acquisition cost**: 30% lower due to security reputation

---

## 📝 Notes

- **Security-First**: All phases prioritize security over feature velocity
- **Compliance-Driven**: SABS, ICASA, SOC2, ISO 27001 certifications guide development
- **User Trust**: Security is the primary differentiator and value proposition
- **Transparency**: Security audit results shared with users (Appendix F)
- **Continuous Improvement**: Weekly security reviews, quarterly penetration tests
- **Community Feedback**: Bug bounty program rewards responsible disclosure
- **Roadmap Flexibility**: Dates may adjust based on security findings and resource availability
- **Documentation**: All security decisions documented for audit trail
- **Budget Allocation**: 20-30% of budget allocated to security in all phases
- **Parallel Development**: Team of 3-4 people enables concurrent workstreams (hardware, firmware, backend, mobile)
- **Pre-Hardware Efficiency**: Mobile app and web dashboard already built with mock data (see `PRE_HARDWARE_COMPLETE.md`)
- **Detailed Planning**: Week-by-week plans established for rapid execution (see `docs/WEEK1_HARDWARE_MASTERY.md`)

**Security Contacts**:
- **Security Team**: security@smartplugai.com
- **Vulnerability Disclosure**: security@smartplugai.com (PGP key available)
- **Bug Bounty**: R5,000 - R50,000 rewards for critical vulnerabilities

**Last Updated**: February 2026  
**Version**: 3.0 (7-Week Phase 1 Timeline)  
**Next Review**: Weekly during Phase 1 (through Week 7), Bi-weekly during Phase 2, Monthly thereafter
